#ifndef MATRIX_H_
#define MATRIX_H_
#include "thread_pool.h"
#include "../Tensor.h"
class matrix_mult_task: public task {
    private:
        Tensor* m_A;
        Tensor* m_B;
        Tensor* m_C;
        int m_a_idx;// A矩阵的行号
        int m_b_idx;// b矩阵的列号
    public:
        matrix_mult_task (Tensor* A, Tensor* B, Tensor* C, int a_idx, int b_idx);
        void run ();
};
class matrix_add_task: public task {
    private:
        Tensor* m_A;
        Tensor* m_B;
        Tensor* m_C;
        int m_thread_id;// 当前任务所在的线程id
        int m_thread_num;// 一共有多少线程
    public:
        matrix_add_task (Tensor* A, Tensor* B, Tensor* C, int thread_id, int thread_num);
        void run ();
};
class matrix_scalar_mult_task: public task {
    private:
        Tensor* m_A;
        float m_scalar;
        Tensor* m_C;
        int m_thread_id;
        int m_thread_num;
    public:
        matrix_scalar_mult_task (Tensor* A, float scalar, Tensor* C, int thread_id, int thread_num);
        void run ();
};
#endif
