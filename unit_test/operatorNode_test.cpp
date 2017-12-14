#include "../include/op_node/Add.h"
#include "../include/op_node/SquareSum.h"
#include "../include/op_node/Minus.h"
#include "../include/op_node/Mult.h"
#include "../include/op_node/Sigmoid.h"
#include "../include/op_node/Parameter.h"
#include "../include/ComputeGraph.h"
#include "../include/Tensor.h"
#include "../include/OperatorNode.h"
#include <vector>
using namespace std;
int main () {
    vector<int> shape1; shape1.push_back (2); shape1.push_back (2);
    vector<int> shape2; shape2.push_back (2); shape2.push_back (2);
    float data1[] = {1,2,3,4};
    float data2[] = {2,3,4,5};
    Tensor* t1 = new Tensor (shape1, data1);
    Tensor* t2 = new Tensor (shape2, data2);
    t1 -> display ();
    t2 -> display ();

    // 生成计算节点
    OperatorNode* p1 = new Parameter ("Parameter", "1", "0", t1);
    OperatorNode* p2 = new Parameter ("Parameter", "2", "0", t2);
    OperatorNode* add = new Add ("Add", "1", "0");
    OperatorNode* minus = new Minus ("Minus", "1", "0");
    OperatorNode* mult = new Mult ("Mult", "1", "0");
    OperatorNode* sigmoid = new Sigmoid ("Sigmoid", "1", "0");
    OperatorNode* square_sum = new SquareSum ("SquareSum", "1", "0");

    // 构建计算图
    ComputeGraph cg;
    cg.add_node ("", p1);
    cg.add_node ("", p2);
    cg.add_node (p1 -> get_name (), add);
    cg.add_node (p2 -> get_name (), add);// 测试加法
    //cg.add_node (p1 -> get_name (), minus);
    //cg.add_node (p2 -> get_name (), minus);// 测试减法
    //cg.add_node (p1 -> get_name (), mult);
    //cg.add_node (p2 -> get_name (), mult);// 测试乘法
    //cg.add_node (p1 -> get_name (), sigmoid);// 测试sigmoid
    //cg.add_node (p1 -> get_name (), square_sum);// 测试squareSum
    // 构建转置图
    cg.build_reverse_graph ();
    
    // 前向传播
    vector<Tensor*> result;
    cg.forward_propagation (result);
    result[0] -> display ();// 前向结果
    // 反向传播
    cg.back_propagation ();
    p1 -> m_sum_grad -> display ();
    p2 -> m_sum_grad -> display ();// 反向结果
    p1 -> m_output -> display ();
    p2 -> m_output -> display ();
}
