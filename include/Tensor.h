#ifndef TENSOR_H_
#define TENSOR_H_
#include <vector>
class Tensor {
    public:
        std::vector<int> m_shape;
        int m_size;
        float* m_tensor;
        Tensor (std::vector<int> shape, int need_init = 1);
        Tensor (std::vector<int> shape, float data[]);
        float get_value (std::vector<int> idxs);// 根据各个维度的下标获取值
        void set_value (std::vector<int> idxs, float value);// 设置某坐标下的值
        Tensor* matrix_mult (Tensor* tensor);// 二维矩阵乘法
        Tensor* scalar_mult (float scalar);// 标量乘法
        void add (Tensor* tensor, Tensor* result);// 累加
        Tensor* add (Tensor* tensor);// 普通加法
        void init ();
        void display ();

        ~Tensor ();
};
#endif
