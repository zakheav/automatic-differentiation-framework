#include "../../include/op_node/SquareSum.h"
using namespace std;
SquareSum::SquareSum (string type, string id, string idx): OperatorNode (type, id, idx) {
}
void SquareSum::op () {
    Tensor* parent_output = ((OperatorNode*) m_parents[0]) -> m_output;
    vector<int> shape; shape.push_back (1); shape.push_back (1);
    m_output = new Tensor (shape);
    for (int i = 0; i < parent_output -> m_size; ++i) {
        m_output -> m_tensor[0] += parent_output -> m_tensor[i] * parent_output -> m_tensor[i];
    }
}
void SquareSum::grad_op () {
    Tensor* parent_output = ((OperatorNode*) m_parents[0]) -> m_output;
    vector<int> shape; shape.push_back (1); shape.push_back (parent_output -> m_size);

    Tensor grad = Tensor (shape);
    vector<int> idxs (2);
    for (int i = 0; i < parent_output -> m_size; ++i) {
        idxs[0] = 0; idxs[1] = i;
        grad.set_value (idxs, parent_output -> m_tensor[i] * 2);
    }
    chain_rule (&grad, 0);
}
