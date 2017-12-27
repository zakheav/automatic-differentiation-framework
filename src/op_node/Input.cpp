#include "../../include/op_node/Input.h"
using namespace std;
Input::Input (string type, string id, string idx, vector<Tensor*> input_data): OperatorNode (type, id, idx) {
    m_data_ptr = 0;
    m_data = input_data;
}
void Input::op () {
    m_output = m_data[m_data_ptr];
    m_data_ptr = (m_data_ptr + 1) % m_data.size ();
}
void Input::release_tensor () {
    if (m_sum_grad != 0) {
        delete m_sum_grad;
        m_sum_grad = 0;
    }
}
Input::~Input () {
    release_tensor ();
}
