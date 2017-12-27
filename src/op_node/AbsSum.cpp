#include "../../include/op_node/AbsSum.h"
#include <math.h>
using namespace std;
AbsSum::AbsSum (string type, string id, string idx): OperatorNode (type, id, idx) {
}
void AbsSum::op () {
    Tensor* parent_output = ((OperatorNode*) m_parents[0]) -> m_output;
    vector<int> shape (2); shape[0] = 1; shape[1] = 1;
    m_output = new Tensor (shape);
    for (int i = 0; i < parent_output -> m_size; ++i) {
        m_output -> m_tensor[0] += fabs (parent_output -> m_tensor[i]);
    }
}
void AbsSum::grad_op () {
    Tensor* parent_output = ((OperatorNode*) m_parents[0]) -> m_output;
    vector<int> shape (2); shape[0] = 1; shape[1] = parent_output -> m_size;

    Tensor grad = Tensor (shape);
    vector<int> idxs (2);
    for (int i = 0; i < parent_output -> m_size; ++i) {
        idxs[0] = 0; idxs[1] = i;
        if (fabs (parent_output -> m_tensor[i] - 0.0) < 0.1) {
            grad.set_value (idxs, 0);
        } else if (parent_output -> m_tensor[i] > 0.0) {
            grad.set_value (idxs, 1);
        } else {
            grad.set_value (idxs, -1);
        }
    }
    chain_rule (&grad, 0);
}
void AbsSum::release_tensor () {
    if (m_sum_grad != 0) {
        delete m_sum_grad;
        m_sum_grad = 0;
    }
    delete m_output;
    m_output = 0;
}
AbsSum::~AbsSum () {
    release_tensor ();
}
