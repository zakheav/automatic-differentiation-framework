#include "../include/op_node/Input.h"
#include "../include/op_node/SquareSum.h"
#include "../include/op_node/Minus.h"
#include "../include/op_node/Mult.h"
#include "../include/op_node/Parameter.h"
#include "../include/VirtualGraph.h"
#include "../include/ComputeGraph.h"
#include "../include/VirtualNode.h"
#include <iostream>
using namespace std;
int main () {
    // 准备数据集
    vector<int> shape_x; shape_x.push_back (4); shape_x.push_back (2);
    float data_x[] = {0,0, 0,1, 1,0, 1,1};
    Tensor* x = new Tensor (shape_x, data_x);

    vector<int> shape_y; shape_y.push_back (4); shape_y.push_back (1);
    float data_y[] = {0,1,1,0};
    Tensor* y = new Tensor (shape_y, data_y);

    vector<int> shape_w1; shape_w1.push_back (2); shape_w1.push_back (4);
    Tensor* w1 = new Tensor (shape_w1);
    w1 -> init ();

    vector<int> shape_w2; shape_w2.push_back (4); shape_w2.push_back (1);
    Tensor* w2 = new Tensor (shape_w2);
    w2 -> init ();

    // 准备虚拟节点
    VirtualNode* input_x = new VirtualNode ("Input", "1");
    input_x -> m_input_data.push_back (x);

    VirtualNode* input_y = new VirtualNode ("Input", "2");
    input_y -> m_input_data.push_back (y);

    VirtualNode* w_1 = new VirtualNode ("Parameter", "1");
    w_1 -> m_data = w1;

    VirtualNode* w_2 = new VirtualNode ("Parameter", "2");
    w_2 -> m_data = w2;

    VirtualNode* mult1 = new VirtualNode ("Mult", "1");
    VirtualNode* mult2 = new VirtualNode ("Mult", "2");
    VirtualNode* sig1 = new VirtualNode ("Sigmoid", "1");
    VirtualNode* sig2 = new VirtualNode ("Sigmoid", "2");
    VirtualNode* minus = new VirtualNode ("Minus", "1");
    VirtualNode* ss = new VirtualNode ("SquareSum", "1");

    // 构建虚拟图
    VirtualGraph vg;
    vg.add_node ("", input_x);
    vg.add_node ("", w_1);
    vg.add_node (input_x -> get_name (), mult1);
    vg.add_node (w_1 -> get_name (), mult1);
    vg.add_node (mult1 -> get_name (), sig1);
    vg.add_node ("", w_2);
    vg.add_node (sig1 -> get_name (), mult2);
    vg.add_node (w_2 -> get_name (), mult2);
    vg.add_node (mult2 -> get_name (), sig2);
    vg.add_node ("", input_y);
    vg.add_node (sig2 -> get_name (), minus);
    vg.add_node (input_y -> get_name (), minus);
    vg.add_node (minus -> get_name (), ss);

    // 生成计算图
    ComputeGraph* train_cg = new ComputeGraph ();
    vg.build_compute_graph (train_cg);
    // 构建转置图，用于反向传播
    train_cg -> build_reverse_graph ();
    // 训练
    for (int i = 0; i < 10000; ++i) {
        vector<Tensor*> error;
        train_cg -> forward_propagation (error);
        train_cg -> back_propagation ();
        if (i % 1000 == 0) {
            error[0] -> display ();
        }
        train_cg -> release_tensor ();// 释放本次迭代的中间结果张量
    }
    // 构建子图
    vector<Node*> endnode_list;
    endnode_list.push_back (sig2);
    vg.build_subgraph (endnode_list);
    ComputeGraph* test_cg = new ComputeGraph ();
    vg.build_compute_graph (test_cg);
    test_cg -> build_reverse_graph ();

    vector<Tensor*> result;
    test_cg -> forward_propagation (result);
    // 输出结果
    cout << "final result:" << endl;
    result[0] -> display ();
}
