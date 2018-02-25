#include "../include/Tensor.h"
#include "../include/op_node/Input.h"
#include "../include/VirtualNode.h"
#include "../include/OperatorNode.h"
#include "../include/VirtualGraph.h"
#include "../include/ComputeGraph.h"
#include "../include/Optimizer.h"
#include "../include/optimizer/Adadelta.h"
#include "../include/BranchNode.h"
#include "../include/LoopNode.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <stdlib.h>
using namespace std;
Tensor* int_to_tensor (int a) {// 把int转化为8位01串，左边是低位，右边是高位
    float* data = new float[8];
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
// rnn中的分支选择函数
Node* choose_node (int idx, Graph* compute_graph, BranchNode* branch_node) {
    ostringstream oss;
    if (idx == 0) {
        int batch_size = 1;
        int hidden_size = 8;
        vector<int> shape (2);
        shape[0]  = batch_size; shape[1] = hidden_size;
        Tensor* init_tensor = new Tensor (shape);
        vector<Tensor*> data; data.push_back (init_tensor);
        Node* init_input = new Input ("Input", "init", "0", data);
        compute_graph -> add_node ("", init_input);
        ((Input*) init_input) -> op ((Input*) init_input);
        return init_input;
    } else {
        oss << (idx - 1) << ":";
        string name = "Mult:h:" + oss.str ();
        return compute_graph -> get_node (name);
    }
}
// rnn中的循环init,condition函数
void init (LoopNode* loop_node) {
}
int condition (Graph* compute_graph, int idx) {
    if (idx < 8) {
        return 0;
    } else {
        return 1;
    }
}
// rnn的输入函数，简单的数据输入预处理
void rnn_input_x (Input* input) {
    int batch_size = 1;
    vector<int> shape (2); shape[0] = batch_size; shape[1] = 2;
    float* data = new float[2 * batch_size];
    for (int i = 0; i < batch_size; ++i) {
        Tensor* a = input -> m_data[input -> m_data_ptr];
        Tensor* b = input -> m_data[input -> m_data_ptr + 1];
        int idx = atoi (input -> m_name[2].c_str ());
        data[i * 2] = a -> m_tensor[idx];
        data[i * 2 + 1] = b -> m_tensor[idx];
        input -> m_data_ptr = (input -> m_data_ptr + 2) % input -> m_data.size ();
    }
    input -> m_output = new Tensor (shape, data);
}
void rnn_input_y (Input* input) {
    int batch_size = 1;
    float* data = new float[1 * batch_size];
    vector<int> shape (2); shape[0] = batch_size; shape[1] = 1;
    for (int i = 0; i < batch_size; ++i) {
        Tensor* a = input -> m_data[input -> m_data_ptr];
        int idx = atoi (input -> m_name[2].c_str ());
        data[i] = a -> m_tensor[idx];
        input -> m_data_ptr = (input -> m_data_ptr + 1) % input -> m_data.size ();
    }
    input -> m_output = new Tensor (shape, data);
}

int main () {
    // 准备数据集
    vector<Tensor*> add_nums;
    vector<Tensor*> sums;
    prepare_data (10000, add_nums, sums);

    int hidden_size = 8;

    vector<int> shape_w1 (2); shape_w1[0] = 2; shape_w1[1] = hidden_size;
    Tensor* t_w1 = new Tensor (shape_w1);
    t_w1 -> init ();

    vector<int> shape_w2 (2); shape_w2[0] = hidden_size; shape_w2[1] = 1;
    Tensor* t_w2 = new Tensor (shape_w2);
    t_w2 -> init ();
    
    vector<int> shape_wh (2); shape_wh[0] = hidden_size; shape_wh[1] = hidden_size;
    Tensor* t_wh = new Tensor (shape_wh);
    t_wh -> init ();

    vector<int> shape_b1 (2); shape_b1[0] = 1; shape_b1[1] = hidden_size;
    Tensor* t_b1 = new Tensor (shape_b1);
    t_b1 -> init ();

    vector<int> shape_b2 (2); shape_b2[0] = 1; shape_b2[1] = 1;
    Tensor* t_b2 = new Tensor (shape_b2);
    t_b2 -> init ();

    // 准备虚拟节点
    VirtualNode* input_x = new VirtualNode ("Input", "1");
    input_x -> m_input_data = add_nums;
    input_x -> input_op = &rnn_input_x;

    VirtualNode* wh = new VirtualNode ("Parameter", "wh", 1);
    wh -> m_data = t_wh;

    VirtualNode* w1 = new VirtualNode ("Parameter", "w1", 1);
    w1 -> m_data = t_w1;

    VirtualNode* multh = new VirtualNode ("Mult", "h");
    VirtualNode* mult1 = new VirtualNode ("Mult", "1");
    BranchNode* branch = new BranchNode ("Branch", "1", &choose_node);
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

    VirtualNode* input_y = new VirtualNode ("Input", "2");
    input_y -> m_input_data = sums;
    input_y -> input_op = &rnn_input_y;
    
    VirtualNode* minus = new VirtualNode ("Minus", "1");
    VirtualNode* abs = new VirtualNode ("AbsSum", "1");

    LoopNode* loop = new LoopNode ("Loop", "1", &init, &condition);

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
    Optimizer* optimizer = new Adadelta (0.2);
    train_cg -> m_optimizer = optimizer;
    // 对计算图进行修剪
    vector<Node*> endnode_list;
    unordered_map<string, Node*>::iterator op_node_map_it = abs -> m_op_node_map.begin ();
    while (op_node_map_it != abs -> m_op_node_map.end ()) {
        endnode_list.push_back (op_node_map_it -> second);
        ++op_node_map_it;
    }
    train_cg -> build_subgraph (endnode_list);
    // 训练
    for (int i = 0; i < 20000; ++i) {
        vector<Node*> error;
        if (i % 1000 == 0) {
            int ptr = ((Input*) (train_cg -> get_node ("Input:1:0:"))) -> m_data_ptr;
            cout << tensor_to_int (((Input*) (train_cg -> get_node ("Input:1:0:"))) -> m_data[ptr]) << "+"
            << tensor_to_int (((Input*) (train_cg -> get_node ("Input:1:0:"))) -> m_data[ptr + 1]);
        }
        train_cg -> forward_propagation (error);
        train_cg -> back_propagation ();
        if (i % 1000 == 0) {
            float r[8] = {0};
            vector<int> r_shape (2); r_shape[0] = 1; r_shape[1] = 8;
            for (int i = 0; i < 8; ++i) {
                ostringstream oss;
                oss << i << ":";
                string op_node_name = sigmoid2 -> get_name () + oss.str ();
                r[i] = ((OperatorNode*) (sigmoid2 -> m_op_node_map[op_node_name])) -> m_output -> m_tensor[0];
            }
            Tensor r_tensor = Tensor (r_shape, r);
            cout << " guess = :" << tensor_to_int (&r_tensor) << endl;
        }
    }

    delete train_cg;
    delete vg;
}
