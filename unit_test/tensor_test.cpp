#include <iostream>
#include <vector>
#include "../include/Tensor.h"
using namespace std;
int main () {
    vector<int> shape (2);
    shape[0] = 2; shape[1] = 2;
    float data[] = {1,2,3,4};
    float data2[] = {2,3,4,5};
    Tensor* tensor1 = new Tensor (shape, data);
    tensor1 -> display ();
    
    Tensor* tensor2 = new Tensor (shape, data2);
    tensor2 -> display ();

    Tensor* random_tensor = new Tensor (shape);
    random_tensor -> init ();
    random_tensor -> display ();// 初始化验证

    Tensor* mult_result = tensor1 -> matrix_mult (tensor2);// 验证乘法
    mult_result -> display ();
      
    Tensor* add_result = tensor1 -> add (tensor2);// 加法验证
    add_result -> display ();
    
    tensor1 -> add (tensor2, add_result);// 加法验证2
    add_result -> display ();

    Tensor* scalar_mult_result = tensor1 -> scalar_mult (3.0);// 标量乘法
    scalar_mult_result -> display ();

    
    vector<int> shape1 (3);
    shape1[0] = 2; shape1[1] = 3; shape1[2] = 2;// {{{1,2},{3,4},{5,6}},{{7,8},{9,10},{11,12}}}
    float data1[] = {1,2,3,4,5,6,7,8,9,10,11,12};
    Tensor* three_dim_tensor = new Tensor (shape1, data1);
    vector<int> idxs (3);
    idxs[0] = 1; idxs[1] = 1; idxs[2] = 0;
    cout << three_dim_tensor -> get_value (idxs) << endl;

}
