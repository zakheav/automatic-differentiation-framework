#include "../../include/op_node/Parameter.h"
using namespace std;
Parameter::Parameter (string type, string id, string idx, Tensor* data, int share_data): OperatorNode (type, id, idx) {
    m_share_data = share_data;
    if (share_data == 0) {
        m_output = new Tensor (data -> m_shape, data -> m_tensor);
    } else {
        m_output = data;
    }
}
void Parameter::release_tensor () {
    if (m_sum_grad != 0) {
        delete m_sum_grad;
        m_sum_grad = 0;
    }
}
Parameter::~Parameter () {
    release_tensor ();
    if (m_share_data == 0) {
        delete m_output;
    }
    m_output = 0;
}
