#include "../include/op_node/Input.h"
#include "../include/op_node/SquareSum.h"
#include "../include/op_node/Minus.h"
#include "../include/op_node/Mult.h"
#include "../include/op_node/Bias.h"
#include "../include/op_node/Parameter.h"
#include "../include/VirtualGraph.h"
#include "../include/ComputeGraph.h"
#include "../include/VirtualNode.h"
#include "../include/Optimizer.h"
#include <iostream>
using namespace std;
int main () {
    // 准备数据集
    vector<Tensor*> data_x_list;
    vector<int> shape_x; shape_x.push_back (1); shape_x.push_back (2);
    float data_x1[] = {0,0};
    float data_x2[] = {0,1};
    float data_x3[] = {1,0};
    float data_x4[] = {1,1};
    data_x_list.push_back (new Tensor (shape_x, data_x1));
    data_x_list.push_back (new Tensor (shape_x, data_x2));
    data_x_list.push_back (new Tensor (shape_x, data_x3));
    data_x_list.push_back (new Tensor (shape_x, data_x4));
    vector<Tensor*> data_y_list;
    vector<int> shape_y; shape_y.push_back (1); shape_y.push_back (1);
    float data_y1[] = {0};
    float data_y2[] = {1};
    float data_y3[] = {1};
    float data_y4[] = {0};
    data_y_list.push_back (new Tensor (shape_y, data_y1));
    data_y_list.push_back (new Tensor (shape_y, data_y2));
    data_y_list.push_back (new Tensor (shape_y, data_y3));
    data_y_list.push_back (new Tensor (shape_y, data_y4));

    vector<int> shape_w1; shape_w1.push_back (2); shape_w1.push_back (4);
    Tensor* w1 = new Tensor (shape_w1);
    w1 -> init ();

    vector<int> shape_w2; shape_w2.push_back (4); shape_w2.push_back (1);
    Tensor* w2 = new Tensor (shape_w2);
    w2 -> init ();

    vector<int> shape_b1; shape_b1.push_back (1); shape_b1.push_back (4);
    Tensor* b1 = new Tensor (shape_b1);
    b1 -> init ();

    vector<int> shape_b2; shape_b2.push_back (1); shape_b2.push_back (1);
    Tensor* b2 = new Tensor (shape_b2);
    b2 -> init ();

    // 准备虚拟节点
    VirtualNode* input_x = new VirtualNode ("Input", "1");
    input_x -> m_input_data = data_x_list;

    VirtualNode* input_y = new VirtualNode ("Input", "2");
    input_y -> m_input_data = data_y_list;

    VirtualNode* w_1 = new VirtualNode ("Parameter", "1");
    w_1 -> m_data = w1;

    VirtualNode* w_2 = new VirtualNode ("Parameter", "2");
    w_2 -> m_data = w2;

    VirtualNode* b_1 = new VirtualNode ("Parameter", "3");
    b_1 -> m_data = b1;

    VirtualNode* b_2 = new VirtualNode ("Parameter", "4");
    b_2 -> m_data = b2;

    VirtualNode* mult1 = new VirtualNode ("Mult", "1");
    VirtualNode* mult2 = new VirtualNode ("Mult", "2");
    VirtualNode* sig1 = new VirtualNode ("Sigmoid", "1");
    VirtualNode* sig2 = new VirtualNode ("Sigmoid", "2");
    VirtualNode* minus = new VirtualNode ("Minus", "1");
    VirtualNode* ss = new VirtualNode ("SquareSum", "1");
    VirtualNode* bias1 = new VirtualNode ("Bias", "1");
    VirtualNode* bias2 = new VirtualNode ("Bias", "2");

    // 构建虚拟图
    VirtualGraph* vg = new VirtualGraph ();
    vg -> add_node ("", input_x);
    vg -> add_node ("", w_1);
    vg -> add_node (input_x -> get_name (), mult1);
    vg -> add_node (w_1 -> get_name (), mult1);
    vg -> add_node ("", b_1);
    vg -> add_node (mult1 -> get_name (), bias1);
    vg -> add_node (b_1 -> get_name (), bias1);
    vg -> add_node (bias1 -> get_name (), sig1);
    vg -> add_node ("", w_2);
    vg -> add_node (sig1 -> get_name (), mult2);
    vg -> add_node (w_2 -> get_name (), mult2);
    vg -> add_node ("", b_2);
    vg -> add_node (mult2 -> get_name (), bias2);
    vg -> add_node (b_2 -> get_name (), bias2);
    vg -> add_node (bias2 -> get_name (), sig2);
    vg -> add_node ("", input_y);
    vg -> add_node (sig2 -> get_name (), minus);
    vg -> add_node (input_y -> get_name (), minus);
    vg -> add_node (minus -> get_name (), ss);

    // 生成计算图
    ComputeGraph* train_cg = new ComputeGraph ();
    vg -> build_compute_graph (train_cg);
    // 初始化优化器，普通sgd
    Optimizer* optimizer = new Optimizer (0.2);
    train_cg -> m_optimizer = optimizer;
    // 构建转置图，用于反向传播
    train_cg -> build_reverse_graph ();
    // 训练
    for (int i = 0; i < 10000; ++i) {
        if (i >= 9900) {
            cout << "input: ";
            int ptr = ((Input*) (train_cg -> get_node ("Input:1:0:"))) -> m_data_ptr;
            ((Input*) (train_cg -> get_node ("Input:1:0:"))) -> m_data[ptr] -> display ();
        }
        vector<Node*> error;
        train_cg -> forward_propagation (error);
        train_cg -> back_propagation ();
        if (i >= 9900) {
            cout << "xor: ";
            ((OperatorNode*) (sig2 -> m_op_node_list[0])) -> m_output -> display (); cout << endl;
        }
        train_cg -> release_tensor ();// 释放本次迭代的中间结果张量
    }

    delete train_cg;
    delete vg;
}
