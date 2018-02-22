#include "../../include/op_node/Input.h"
using namespace std;
void input_default_op (Input* input) {
    input -> m_output = input -> m_data[input -> m_data_ptr];
    input -> m_data_ptr = (input -> m_data_ptr + 1) % input -> m_data.size ();
}
Input::Input (string type, string id, string idx, vector<Tensor*> input_data, void (*func) (Input*)): OperatorNode (type, id, idx) {
    m_data_ptr = 0;
    m_data = input_data;
    if (func == 0) {
        op = &input_default_op;
    } else {
        op = func;
    }
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
