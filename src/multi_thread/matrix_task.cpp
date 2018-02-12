#include "../../include/multi_thread/matrix_task.h"
using namespace std;
/*矩阵乘法子任务*/
matrix_mult_task::matrix_mult_task (Tensor* A, Tensor* B, Tensor* C, int a_idx, int b_idx) {
    m_A = A;
    m_B = B;
    m_C = C;
    m_a_idx = a_idx;
    m_b_idx = b_idx;
}

void matrix_mult_task::run () {
    int a_col = m_A -> m_shape[1];
    float c = 0;
    float compensation = 0.0;
    int idx0 = 0, idx1 = 0, idx2 = 0;
    for (int i = 0; i < a_col; ++i) {
        idx0 = m_a_idx * m_A -> m_shape[1] + i;
        idx1 = i * m_B -> m_shape[1] + m_b_idx;
        float y = m_A -> m_tensor[idx0] * m_B -> m_tensor[idx1] - compensation;// 补偿
        float t = c + y;// 发生舍入
        compensation = (t - c) - y;// 记录下舍入误差
        c = t;
    }
    idx2 = m_a_idx * m_C -> m_shape[1] + m_b_idx;
    m_C -> m_tensor[idx2] = c;
}

/*矩阵加法子任务*/
matrix_add_task::matrix_add_task (Tensor* A, Tensor* B, Tensor* C, int thread_id, int thread_num) {
    m_A = A;
    m_B = B;
    m_C = C;
    m_thread_id = thread_id;
    m_thread_num = thread_num;
}

void matrix_add_task::run () {
    for (int idx = m_thread_id; idx < m_A -> m_size; idx += m_thread_num) {
        m_C -> m_tensor[idx] = m_A -> m_tensor[idx] + m_B -> m_tensor[idx];
    }
}

/*矩阵标量乘法子任务*/
matrix_scalar_mult_task::matrix_scalar_mult_task (Tensor* A, float scalar, Tensor* C, int thread_id, int thread_num) {
    m_A = A;
    m_scalar = scalar;
    m_C = C;
    m_thread_id = thread_id;
    m_thread_num = thread_num;
}

void matrix_scalar_mult_task::run () {
    for (int idx = m_thread_id; idx < m_A -> m_size; idx += m_thread_num) {
        m_C -> m_tensor[idx] = m_A -> m_tensor[idx] * m_scalar;
    }
}
