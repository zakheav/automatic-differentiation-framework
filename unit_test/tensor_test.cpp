#include <iostream>
#include <vector>
#include "../include/Tensor.h"
using namespace std;
int main () {
    vector<int> shape (2);
    shape[0] = 20; shape[1] = 10;

    Tensor* tensor1 = new Tensor (shape);
    tensor1 -> init ();
    tensor1 -> display (); cout << endl;
    
    Tensor* tensor2 = new Tensor (shape);
    tensor2 -> init ();
    tensor2 -> display (); cout << endl;

    Tensor* random_tensor = new Tensor (shape);
    random_tensor -> init ();
    cout << "init test" << endl;
    //random_tensor -> display ();// 初始化验证

    Tensor* mult_result = tensor1 -> matrix_mult (tensor2);// 验证乘法
    cout << "matrix mult test" << endl;
    //mult_result -> display ();
      
    Tensor* add_result = tensor1 -> add (tensor2);// 加法验证
    cout << "add test 1" << endl;
    //add_result -> display ();
    
    tensor1 -> add (tensor2, add_result);// 加法验证2
    cout << "add test 2" << endl;
    //add_result -> display ();
    
    Tensor* ele_mult_result = tensor1 -> element_mult (tensor2);
    cout << "element mult test" << endl;
    //ele_mult_result -> display ();

    Tensor* scalar_mult_result = tensor1 -> scalar_mult (3.0);// 标量乘法
    cout << "scalar mult test" << endl;
    scalar_mult_result -> display ();

    tensor1 -> scalar_acc_mult (2.0);// 标量累乘
    cout << "scalar acc mult test" << endl;
    tensor1 -> display ();
    /*
    tensor1 -> element_square ();
    cout << "element square test" << endl;
    tensor1 -> display ();
    
    vector<int> shape1 (3);
    shape1[0] = 2; shape1[1] = 3; shape1[2] = 2;// {{{1,2},{3,4},{5,6}},{{7,8},{9,10},{11,12}}}
    float data1[] = {1,2,3,4,5,6,7,8,9,10,11,12};
    Tensor* three_dim_tensor = new Tensor (shape1, data1);
    vector<int> idxs (3);
    idxs[0] = 1; idxs[1] = 1; idxs[2] = 0;
    cout << "index value test" << endl;
    cout << three_dim_tensor -> get_value (idxs) << endl;
    */
}
