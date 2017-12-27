#include "../../include/op_node/Mult.h"
#include <iostream>
using namespace std;
Mult::Mult (string type, string id, string idx): OperatorNode (type, id, idx) {
}
void Mult::op () {
    Tensor* parent0_output = ((OperatorNode*) m_parents[0]) -> m_output;
    Tensor* parent1_output = ((OperatorNode*) m_parents[1]) -> m_output;
    m_output = parent0_output -> matrix_mult (parent1_output);
    if (m_output == 0) {
        cout << "shape is not match:" << get_name () << endl;
    }
}
void Mult::grad_op () {
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

    vector<int> idxs0 (2);
    vector<int> idxs1 (2);
    // grad0
    for (int i = 0; i < m_output -> m_shape[0]; ++i) {
        for (int j = 0; j < m_output -> m_shape[1]; ++j) {
            for (int l = 0; l < parent0_output -> m_shape[0]; ++l) {
                for (int m = 0; m < parent0_output -> m_shape[1]; ++m) {
                    idxs0[0] = i * m_output -> m_shape[1] + j;
                    idxs0[1] = l * parent0_output -> m_shape[1] + m;
                    if (i == l) {
                        idxs1[0] = m;
                        idxs1[1] = j;
                        grad0.set_value (idxs0, parent1_output -> get_value (idxs1));
                    } else {
                        grad0.set_value (idxs0, 0);
                    }
                }
            }
        }
    }
    // grad1
    for (int i = 0; i < m_output -> m_shape[0]; ++i) {
        for (int j = 0; j < m_output -> m_shape[1]; ++j) {
            for (int l = 0; l < parent1_output -> m_shape[0]; ++l) {
                for (int m = 0; m < parent1_output -> m_shape[1]; ++m) {
                    idxs0[0] = i * m_output -> m_shape[1] + j;
                    idxs0[1] = l * parent1_output -> m_shape[1] + m;
                    if (j == m) {
                        idxs1[0] = i;
                        idxs1[1] = l;
                        grad1.set_value (idxs0, parent0_output -> get_value (idxs1));
                    } else {
                        grad1.set_value (idxs0, 0);
                    }
                }
            }
        }
    }
    // chain rule
    chain_rule (&grad0, 0);
    chain_rule (&grad1, 1);
}
void Mult::release_tensor () {
    if (m_sum_grad != 0) {
        delete m_sum_grad;
        m_sum_grad = 0;
    }
    delete m_output;
    m_output = 0;
}
Mult::~Mult () {
    release_tensor ();
}
