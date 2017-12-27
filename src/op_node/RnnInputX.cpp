#include "../../include/op_node/RnnInputX.h"
#include <stdlib.h>
#include <iostream>
using namespace std;
RnnInputX::RnnInputX (string type, string id, string idx, vector<Tensor*> input_data): OperatorNode (type, id, idx) {
    m_data_ptr = 0;
    m_data = input_data;
}
void RnnInputX::op () {
    int batch_size = 1;
    vector<int> shape (2); shape[0] = batch_size; shape[1] = 2;
    float* data = (float*) malloc (sizeof (float) * 2 * batch_size);
    for (int i = 0; i < batch_size; ++i) {
        Tensor* a = m_data[m_data_ptr];
        Tensor* b = m_data[m_data_ptr + 1];
        int idx = atoi (m_name[2].c_str ());
        data[i * 2] = a -> m_tensor[idx];
        data[i * 2 + 1] = b -> m_tensor[idx];
        m_data_ptr = (m_data_ptr + 2) % m_data.size ();
    }
    m_output = new Tensor (shape, data);
}
void RnnInputX::release_tensor () {
    if (m_sum_grad != 0) {
        delete m_sum_grad;
        m_sum_grad = 0;
    }
    delete m_output;
    m_output = 0;
}
RnnInputX::~RnnInputX () {
    release_tensor ();
}
