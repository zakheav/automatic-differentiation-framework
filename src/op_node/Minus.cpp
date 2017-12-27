#include "../../include/op_node/Minus.h"
using namespace std;
Minus::Minus (string type, string id, string idx): OperatorNode (type, id, idx) {
}
void Minus::op () {
    Tensor* parent0_output = ((OperatorNode*) m_parents[0]) -> m_output;
    Tensor* parent1_output = ((OperatorNode*) m_parents[1]) -> m_output;
    m_output = parent1_output -> scalar_mult (-1.0);
    parent0_output -> add (m_output, m_output);
}
void Minus::grad_op () {
    Tensor* parent0_output = ((OperatorNode*) m_parents[0]) -> m_output;
    Tensor* parent1_output = ((OperatorNode*) m_parents[1]) -> m_output;
    vector<int> shape0 (2);
    shape0[0] = m_output -> m_size;
    shape0[1] = parent0_output -> m_size;
    vector<int> shape1 (2);
    shape1[0] = m_output -> m_size;
    shape1[1] = parent1_output -> m_size;

    Tensor grad0 = Tensor (shape0);
    Tensor grad1 = Tensor (shape1);
    
    vector<int> idxs (2);
    // grad0
    for (int i = 0; i < m_output -> m_size; ++i) {
        for (int j = 0; j < parent0_output -> m_size; ++j) {
            if (i == j) {
                idxs[0] = i; idxs[1] = j;
                grad0.set_value (idxs, 1);
            }
        }
    }
    // grad1
    for (int i = 0; i < m_output -> m_size; ++i) {
        for (int j = 0; j < parent1_output -> m_size; ++j) {
            if (i == j) {
                idxs[0] = i; idxs[1] = j;
                grad1.set_value (idxs, -1);
            }
        }
    }

    // chain rule
    chain_rule (&grad0, 0);
    chain_rule (&grad1, 1);
}
void Minus::release_tensor () {
    if (m_sum_grad != 0) {
        delete m_sum_grad;
        m_sum_grad = 0;
    }
    delete m_output;
    m_output = 0;
}
Minus::~Minus () {
    release_tensor ();
}
