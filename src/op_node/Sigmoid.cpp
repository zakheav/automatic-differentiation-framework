#include "../../include/op_node/Sigmoid.h"
#include <cmath>
using namespace std;
Sigmoid::Sigmoid (string type, string id, string idx): OperatorNode (type, id, idx) {
}
void Sigmoid::op () {
    Tensor* parent_output = ((OperatorNode*) m_parents[0]) -> m_output;
    m_output = new Tensor (parent_output -> m_shape);
    for (int i = 0; i < parent_output -> m_size; ++i) {
        m_output -> m_tensor[i] = 1.0 / (1 + pow (2.718, 0 - parent_output -> m_tensor[i]));
    }
}
void Sigmoid::grad_op () {
    Tensor* parent_output = ((OperatorNode*) m_parents[0]) -> m_output;
    vector<int> shape (2); shape[0] = m_output -> m_size; shape[1] = parent_output -> m_size;
    Tensor grad = Tensor (shape);
    
    vector<int> idxs (2);
    // grad
    for (int i = 0; i < m_output -> m_size; ++i) {
        for (int j = 0; j < parent_output -> m_size; ++j) {
            idxs[0] = i;
            idxs[1] = j;
            if (i == j) {
                grad.set_value (idxs, m_output -> m_tensor[i] * (1 - m_output -> m_tensor[i]));
            }
        }
    }
    chain_rule (&grad, 0);
}
