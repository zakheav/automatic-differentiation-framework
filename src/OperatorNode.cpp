#include "../include/OperatorNode.h"
#include <iostream>
using namespace std;
OperatorNode::OperatorNode (string type, string id, string idx): Node (type, id) {
    m_name.push_back (idx);
    m_a = 0.2;
    m_sum_grad = 0;
    m_output = 0;
}
void OperatorNode::chain_rule (Tensor* grad, int parent_idx) {
    OperatorNode* parent_op_node = (OperatorNode*)  m_parents[parent_idx];
    if (parent_op_node -> m_sum_grad == 0) {
        if (m_sum_grad == 0) {
            // 把grad压缩为一行
            vector<int> shape (2); shape[0] = 1; shape[1] = grad -> m_shape[1];
            parent_op_node -> m_sum_grad = new Tensor (shape);
            for (int i = 0; i < grad -> m_size; ++i) {
                int idx = i % parent_op_node -> m_sum_grad -> m_size;
                parent_op_node -> m_sum_grad -> m_tensor[idx] += grad -> m_tensor[i];
            } 
        } else {
            parent_op_node -> m_sum_grad = m_sum_grad -> matrix_mult (grad);
        }
    } else {
        if (m_sum_grad == 0) {
            // 把grad压缩为一行
            for (int i = 0; i < grad -> m_size; ++i) {
                int idx = i % parent_op_node -> m_sum_grad -> m_size;
                parent_op_node -> m_sum_grad -> m_tensor[idx] += grad -> m_tensor[i];
            }
        } else {
            parent_op_node -> m_sum_grad -> add (m_sum_grad -> matrix_mult (grad), parent_op_node -> m_sum_grad);
        }
    }
}
void OperatorNode::op () {
}
void OperatorNode::grad_op () {
}
void OperatorNode::update () {
    if (m_sum_grad != 0 && m_name[0] == "Parameter") {
        for (int i = 0; i < m_output -> m_size; ++i) {
            m_output -> m_tensor[i] -= m_a * m_sum_grad -> m_tensor[i];
        }
    }
}
void OperatorNode::release_tensor () {
}
OperatorNode::~OperatorNode () {
    // cout << "free operatorNode:" << get_name () << endl;
}
