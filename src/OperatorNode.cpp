#include "../include/OperatorNode.h"
#include <iostream>
using namespace std;
OperatorNode::OperatorNode (string type, string id, string idx): Node (type, id) {
    m_name.push_back (idx);
    m_a = 0.1;
    m_sum_grad = 0;
    m_output = 0;
}
void OperatorNode::chain_rule (Tensor* grad, int parent_idx) {
    OperatorNode* parent_op_node = (OperatorNode*)  m_parents[parent_idx];
    if (parent_op_node -> m_sum_grad == 0) {
        if (m_sum_grad == 0) {
            parent_op_node -> m_sum_grad = new Tensor (grad -> m_shape, grad -> m_tensor); 
        } else {
            parent_op_node -> m_sum_grad = m_sum_grad -> matrix_mult (grad);
        }
    } else {
        if (m_sum_grad == 0) {
            parent_op_node -> m_sum_grad -> add (grad, parent_op_node -> m_sum_grad);
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
        vector<int> idxs (2);
        for (int i = 0; i < m_sum_grad -> m_shape[0]; ++i) {
            for (int j = 0; j < m_output -> m_size; ++j) {
                idxs[0] = i; idxs[1] = j;
                m_output -> m_tensor[j] = m_output -> m_tensor[j] - m_a * m_sum_grad -> get_value (idxs);
            }
        }
    }
}
OperatorNode::~OperatorNode () {
    // cout << "free operatorNode:" << get_name () << endl;
    delete m_sum_grad;
    if (m_name[0] != "Parameter") {
        delete m_output;
    }
}
