#include "../../include/optimizer/Adadelta.h"
#include "../../include/OperatorNode.h"
#include <math.h>
#include <iostream>
using namespace std;
Adadelta::Adadelta (float a): Optimizer (a) {
    m_epsl = 0.5;
    m_lambda = 0.3;
}
void Adadelta::optimize (vector<Node*> &topo_results) {
    for (int i = 0; i < topo_results.size (); ++i) {// 计算梯度
        ((OperatorNode*) topo_results[i]) -> grad_op ();
    }
    for (int i = 0; i < topo_results.size (); ++i) {// 更新
        OperatorNode* op_node = (OperatorNode*) topo_results[i];
        if (op_node -> m_sum_grad != 0 && op_node -> m_name[0] == "Parameter") {
            string name = op_node -> get_name ();
            if (m_tensor_store.find (name) == m_tensor_store.end ()) {// 不存在累计梯度
                for (int j = 0; j < op_node -> m_output -> m_size; ++j) {
                    op_node -> m_output -> m_tensor[j] -= m_a * op_node -> m_sum_grad -> m_tensor[j];
                }
                Tensor* tensor = new Tensor (op_node -> m_sum_grad -> m_shape, op_node -> m_sum_grad -> m_tensor);
                tensor -> element_square ();
                m_tensor_store[name] = tensor;
            } else {
                Tensor* acc_grad = m_tensor_store[name];
                for (int j = 0; j < acc_grad -> m_size; ++j) {
                    op_node -> m_output -> m_tensor[j] -= m_a * op_node -> m_sum_grad -> m_tensor[j]
                                                          / (sqrt (acc_grad -> m_tensor[j]) + m_epsl);
                }
                // 更新累积梯度
                op_node -> m_sum_grad -> element_square ();
                op_node -> m_sum_grad -> scalar_acc_mult (m_lambda);
                acc_grad -> scalar_acc_mult (1 - m_lambda);
                acc_grad -> add (op_node -> m_sum_grad, acc_grad);
            }
        }
    }
}
Adadelta::~Adadelta () {
    unordered_map <string, Tensor*>::iterator tensor_store_it = m_tensor_store.begin ();
    while (tensor_store_it != m_tensor_store.end ()) {
        delete tensor_store_it -> second;
        ++tensor_store_it;
    }
    m_tensor_store.clear ();
}
