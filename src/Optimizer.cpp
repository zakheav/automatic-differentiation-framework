#include "../include/Optimizer.h"
#include "../include/OperatorNode.h"
#include "../include/Tensor.h"
#include <iostream>
using namespace std;
Optimizer::Optimizer (float a) {
    m_a = a;
}
void Optimizer::optimize (vector<Node*> &topo_results) {
    for (int i = 0; i < topo_results.size (); ++i) {// 计算梯度
        ((OperatorNode*) topo_results[i]) -> grad_op ();
    }
    for (int i = 0; i < topo_results.size (); ++i) {// 更新
        OperatorNode* op_node = (OperatorNode*) topo_results[i];
        if (op_node -> m_sum_grad != 0 && op_node -> m_name[0] == "Parameter") {
            for (int j = 0; j < op_node -> m_output -> m_size; ++j) {
                op_node -> m_output -> m_tensor[j] -= m_a * op_node -> m_sum_grad -> m_tensor[j];
            }
        }
    }
}
Optimizer::~Optimizer () {
    cout << "free optimizer" << endl;
}
