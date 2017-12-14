#include <iostream>
#include <stdlib.h>
#include "../include/Tensor.h"
using namespace std;
Tensor::Tensor (vector<int> shape) {
    vector<int>::iterator shape_it = shape.begin ();
    m_size = 1;
    while (shape_it != shape.end ()) {
        m_shape.push_back (*shape_it);
        m_size *= (*shape_it);
        ++shape_it;
    }
    m_tensor = (float*) malloc (m_size * sizeof (float));
    for (int i = 0; i < m_size; ++i) {
        m_tensor[i] = 0.0;
    }
}

Tensor::Tensor (vector<int> shape, float data[]) {
    vector<int>::iterator shape_it = shape.begin ();
    m_size = 1;
    while (shape_it != shape.end ()) {
        m_shape.push_back (*shape_it);
        m_size *= (*shape_it);
        ++shape_it;
    }
    m_tensor = (float*) malloc (m_size * sizeof (float));
    for (int i = 0; i < m_size; ++i) {
        m_tensor[i] = data[i];
    }
}

float Tensor::get_value (vector<int> idxs) {
    int idx = 0;
    int t = 1;
    for (int i = idxs.size () - 1; i >= 0; --i) {
        idx += idxs[i] * t;
        t *= m_shape[i];
    }
    return m_tensor[idx];
}

void Tensor::set_value (vector<int> idxs, float value) {
    int idx = 0;
    int t = 1;
    for (int i = idxs.size () - 1; i >= 0; --i) {
        idx += idxs[i] * t;
        t *= m_shape[i];
    }
    m_tensor[idx] = value;
}

Tensor* Tensor::matrix_mult (Tensor* tensor) {
    vector<int> idxs0 (2);
    vector<int> idxs1 (2);
    vector<int> idxs2 (2);
    Tensor* result = 0;
    if (m_shape[1] == tensor -> m_shape[0]) {
        vector<int> result_shape;
        result_shape.push_back (m_shape[0]);
        result_shape.push_back (tensor -> m_shape[1]);
        result = new Tensor (result_shape);
        for (int i = 0; i < m_shape[0]; ++i) {
            for (int j = 0; j < tensor -> m_shape[1]; ++j) {
                for (int k = 0; k < m_shape[1]; ++k) {
                    idxs0[0] = i; idxs0[1] = k;
                    idxs1[0] = k; idxs1[1] = j;
                    idxs2[0] = i; idxs2[1] = j;
                    result -> set_value (idxs2, result -> get_value (idxs2) + get_value (idxs0) * tensor -> get_value (idxs1));
                }
            }
        }
    }
    return result;
}

Tensor* Tensor::scalar_mult (float scalar) {
    Tensor* result = new Tensor (m_shape);
    for (int i = 0; i < m_size; ++i) {
        result -> m_tensor[i] = m_tensor[i] * scalar;
    }
    return result;
}

void Tensor::add (Tensor* tensor, Tensor* result) {
    for (int i = 0; i < m_size; ++i) {
        result -> m_tensor[i] = m_tensor[i] + tensor -> m_tensor[i];
    }
}

Tensor* Tensor::add (Tensor* tensor) {
    Tensor* result = 0;
    int same_shape = 1;
    if (m_shape.size () == tensor -> m_shape.size ()) {
        for (int i = 0; i < m_shape.size (); ++i) {
            if (m_shape[i] != tensor -> m_shape[i]) {
                same_shape = 0;
                break;
            }
        }
    } else {
        same_shape = 0;
    }

    if (same_shape == 1) {
        result = new Tensor (tensor -> m_shape);
        for (int i = 0; i < m_size; ++i) {
            result -> m_tensor[i] = m_tensor[i] + tensor -> m_tensor[i];
        }
    }
    return result;
}

void Tensor::init () {
    for (int i = 0; i < m_size; ++i) {
        m_tensor[i] = (rand () % 100) / 1000.0;
    }
}

void Tensor::display () {
    vector<int> idxs0 (2);
    for (int i = 0; i < m_shape[0]; ++i) {
        for (int j = 0; j < m_shape[1]; ++j) {
            idxs0[0] = i; idxs0[1] = j;
            cout << get_value (idxs0) << " ";
        }
        cout << endl;
    }
    cout << endl;
}

Tensor::~Tensor () {
    delete m_tensor;
}
