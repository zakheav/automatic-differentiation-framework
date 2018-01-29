#include "../../include/op_node/SquareSum.h"
using namespace std;
SquareSum::SquareSum (string type, string id, string idx): OperatorNode (type, id, idx) {
}
void SquareSum::op () {
    Tensor* parent_output = ((OperatorNode*) m_parents[0]) -> m_output;
    vector<int> shape (2); shape[0] = 1; shape[1] = 1;
    m_output = new Tensor (shape);
    m_output -> m_tensor[0] = parent_output -> element_square_sum ();
}
void SquareSum::grad_op () {
    Tensor* parent_output = ((OperatorNode*) m_parents[0]) -> m_output;
    vector<int> shape (2); shape[0] = 1; shape[1] = parent_output -> m_size;

    Tensor grad = Tensor (shape);
    vector<int> idxs (2);
    for (int i = 0; i < parent_output -> m_size; ++i) {
        idxs[0] = 0; idxs[1] = i;
        grad.set_value (idxs, parent_output -> m_tensor[i] * 2);
    }
    chain_rule (&grad, 0);
}
void SquareSum::release_tensor () {
    if (m_sum_grad != 0) {
        delete m_sum_grad;
        m_sum_grad = 0;
    }
    delete m_output;
    m_output = 0;
}
SquareSum::~SquareSum () {
    release_tensor ();
}
