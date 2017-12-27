#include "../../include/Tensor.h"
#include "../../include/op_node/RnnInputX.h"
#include "../../include/VirtualNode.h"
#include "../../include/OperatorNode.h"
#include "../../include/VirtualGraph.h"
#include "../../include/ComputeGraph.h"
#include "RnnLoop.h"
#include "RnnBranch.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
using namespace std;
Tensor* int_to_tensor (int a) {// 把int转化为8位01串，左边是低位，右边是高位
    float* data = (float*) malloc (8 * sizeof (float));
    vector<int> shape (2); shape[0] = 1; shape[1] = 8;
    int mask = 1;
    for (int i = 0; i < 8; ++i) {
        if ((mask & a) == 0) {
            data[i] = 0;
        } else {
            data[i] = 1;
        }
        mask = mask << 1;
    }
    return new Tensor (shape, data);
}
int tensor_to_int (Tensor* tensor) {
    int result = 0;
    for (int i = 7; i >= 0; --i) {
        int a = tensor -> m_tensor[i] < 0.5 ? 0 : 1;
        result = result * 2 + a;
    }
    return result;
}
void prepare_data (int num, vector<Tensor*> &add_nums, vector<Tensor*> &sums) {
    for (int i = 0; i < num; ++i) {
        int a = rand () % 128;
        int b = rand () % 128;
        int c = a + b;
        Tensor* t_a = int_to_tensor (a);
        Tensor* t_b = int_to_tensor (b);
        Tensor* t_c = int_to_tensor (c);
        add_nums.push_back (t_a);
        add_nums.push_back (t_b);
        sums.push_back (t_c);
    }
}
int main () {
    // 准备数据集
    vector<Tensor*> add_nums;
    vector<Tensor*> sums;
    prepare_data (10000, add_nums, sums);
    
    vector<int> shape_w1 (2); shape_w1[0] = 2; shape_w1[1] = 4;
    Tensor* t_w1 = new Tensor (shape_w1);
    t_w1 -> init ();

    vector<int> shape_w2 (2); shape_w2[0] = 4; shape_w2[1] = 1;
    Tensor* t_w2 = new Tensor (shape_w2);
    t_w2 -> init ();
    
    vector<int> shape_wh (2); shape_wh[0] = 4; shape_wh[1] = 4;
    Tensor* t_wh = new Tensor (shape_wh);
    t_wh -> init ();

    vector<int> shape_b1 (2); shape_b1[0] = 1; shape_b1[1] = 4;
    Tensor* t_b1 = new Tensor (shape_b1);
    t_b1 -> init ();

    vector<int> shape_b2 (2); shape_b2[0] = 1; shape_b2[1] = 1;
    Tensor* t_b2 = new Tensor (shape_b2);
    t_b2 -> init ();

    // 准备虚拟节点
    VirtualNode* input_x = new VirtualNode ("RnnInputX", "1");
    input_x -> m_input_data = add_nums;

    VirtualNode* wh = new VirtualNode ("Parameter", "wh", 1);
    wh -> m_data = t_wh;

    VirtualNode* w1 = new VirtualNode ("Parameter", "w1", 1);
    w1 -> m_data = t_w1;

    VirtualNode* multh = new VirtualNode ("Mult", "h");
    VirtualNode* mult1 = new VirtualNode ("Mult", "1");
    RnnBranch* branch = new RnnBranch ("Branch", "1");
    VirtualNode* add1 = new VirtualNode ("Add", "1");

    VirtualNode* b1 = new VirtualNode ("Parameter", "b1", 1);
    b1 -> m_data = t_b1;
    
    VirtualNode* bias1 = new VirtualNode ("Bias", "1");
    VirtualNode* sigmoid1 = new VirtualNode ("Sigmoid", "1");
    
    VirtualNode* w2 = new VirtualNode ("Parameter", "w2", 1);
    w2 -> m_data = t_w2;

    VirtualNode* mult2 = new VirtualNode ("Mult", "2");
    
    VirtualNode* b2 = new VirtualNode ("Parameter", "b2", 1);
    b2 -> m_data = t_b2;

    VirtualNode* bias2 = new VirtualNode ("Bias", "2");
    VirtualNode* sigmoid2 = new VirtualNode ("Sigmoid", "2");

    VirtualNode* input_y = new VirtualNode ("RnnInputY", "1");
    input_y -> m_input_data = sums;
    
    VirtualNode* minus = new VirtualNode ("Minus", "1");
    VirtualNode* abs = new VirtualNode ("AbsSum", "1");

    RnnLoop* loop = new RnnLoop ("Loop", "1");

    // 构建虚拟图
    loop -> m_sub_vgraph -> add_node ("", input_x);
    loop -> m_sub_vgraph -> add_node ("", w1);
    loop -> m_sub_vgraph -> add_node (input_x -> get_name (), mult1);
    loop -> m_sub_vgraph -> add_node (w1 -> get_name (), mult1);
    loop -> m_sub_vgraph -> add_node ("", branch);
    loop -> m_sub_vgraph -> add_node (branch -> get_name (), add1);
    loop -> m_sub_vgraph -> add_node (mult1 -> get_name (), add1);
    loop -> m_sub_vgraph -> add_node ("", b1);
    loop -> m_sub_vgraph -> add_node (add1 -> get_name (), bias1);
    loop -> m_sub_vgraph -> add_node (b1 -> get_name (), bias1);
    loop -> m_sub_vgraph -> add_node (bias1 -> get_name (), sigmoid1);

    loop -> m_sub_vgraph -> add_node ("", wh);// 为下一次循环做准备
    loop -> m_sub_vgraph -> add_node (sigmoid1 -> get_name (), multh);
    loop -> m_sub_vgraph -> add_node (wh -> get_name (), multh);

    loop -> m_sub_vgraph -> add_node ("", w2);
    loop -> m_sub_vgraph -> add_node (sigmoid1 -> get_name (), mult2);
    loop -> m_sub_vgraph -> add_node (w2 -> get_name (), mult2);
    loop -> m_sub_vgraph -> add_node ("", b2);
    loop -> m_sub_vgraph -> add_node (mult2 -> get_name (), bias2);
    loop -> m_sub_vgraph -> add_node (b2 -> get_name (), bias2);
    loop -> m_sub_vgraph -> add_node (bias2 -> get_name (), sigmoid2);
    loop -> m_sub_vgraph -> add_node ("", input_y);
    loop -> m_sub_vgraph -> add_node (sigmoid2 -> get_name (), minus);
    loop -> m_sub_vgraph -> add_node (input_y -> get_name (), minus);
    loop -> m_sub_vgraph -> add_node (minus -> get_name (), abs);
    
    VirtualGraph* vg = new VirtualGraph ();
    vg -> add_node ("", loop);

    // 构建计算图
    ComputeGraph* train_cg = new ComputeGraph ();
    vg -> build_compute_graph (train_cg);
    // 构建转置图
    train_cg -> build_reverse_graph ();
    // 构建子图
    train_cg -> build_subgraph (abs -> m_op_node_list);
    // 训练
    for (int i = 0; i < 20000; ++i) {
        vector<Node*> error;
        if (i % 1000 == 0) {
            int ptr = ((RnnInputX*) (train_cg -> get_node ("RnnInputX:1:0:"))) -> m_data_ptr;
            cout << tensor_to_int (((RnnInputX*) (train_cg -> get_node ("RnnInputX:1:0:"))) -> m_data[ptr]) << "+"
            << tensor_to_int (((RnnInputX*) (train_cg -> get_node ("RnnInputX:1:0:"))) -> m_data[ptr + 1]);
        }
        train_cg -> forward_propagation (error);
        train_cg -> back_propagation ();
        if (i % 1000 == 0) {
            float r[8] = {0};
            vector<int> r_shape (2); r_shape[0] = 1; r_shape[1] = 8;
            for (int i = 0; i < sigmoid2 -> m_op_node_list.size (); ++i) {
                r[i] = ((OperatorNode*) (sigmoid2 -> m_op_node_list[i])) -> m_output -> m_tensor[0];
            }
            Tensor r_tensor = Tensor (r_shape, r);
            cout << " guess = :" << tensor_to_int (&r_tensor) << endl;
        }
        train_cg -> release_tensor ();// 释放本次迭代的中间结果张量
    }
}
