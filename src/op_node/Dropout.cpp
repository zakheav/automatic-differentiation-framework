#include "../../include/op_node/Dropout.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
Dropout::Dropout (string type, string id, string idx, Tensor* filter, float keep_rate): OperatorNode (type, id, idx) {
    m_filter = new Tensor (filter -> m_shape, 0);
    m_keep_rate = keep_rate;
}
void Dropout::op () {
    float n = 1.0 / m_keep_rate;
    srand (time (0));
    for (int i = 0; i < m_filter -> m_size; ++i) {
        float r = (rand () % 1000) / 1000.0;
        if (r > m_keep_rate) {
            m_filter -> m_tensor[i] = 0.0;
        } else {
            m_filter -> m_tensor[i] = n;
        }
    }

    Tensor* parent0_output = ((OperatorNode*) m_parents[0]) -> m_output;
    m_output = parent0_output -> element_mult (m_filter);
    if (m_output == 0) {
        cout << "filter size error" << endl;
    }
}
void Dropout::grad_op () {
    Tensor* parent_output = ((OperatorNode*) m_parents[0]) -> m_output;
    vector<int> shape (2); shape[0] = m_output -> m_size; shape[1] = parent_output -> m_size;

    Tensor grad = Tensor (shape);
    vector<int> idxs (2);
    for (int i = 0; i < m_output -> m_size; ++i) {
        for (int j = 0; j < parent_output -> m_size; ++j) {
            if (i == j) {
                idxs[0] = i; idxs[1] = j;
                grad.set_value (idxs, m_filter -> m_tensor[i]);
            }
        }
    }
    // chain rule
    chain_rule (&grad, 0);
}
void Dropout::release_tensor () {
    if (m_sum_grad != 0) {
        delete m_sum_grad;
        m_sum_grad = 0;
    }
    delete m_output;
    m_output = 0;
}
Dropout::~Dropout () {
    release_tensor ();
    delete m_filter;
    m_filter = 0;
}
