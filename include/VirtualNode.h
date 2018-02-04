#ifndef VIRTUALNODE_H_
#define VIRTUALNODE_H_
#include "Node.h"
#include "Tensor.h"
#include "OperatorNode.h"
#include "Graph.h"
#include <string>
#include <unordered_map>
class VirtualNode: public Node {
    public:
        Tensor* m_data;// 记录一些必要的数据，比如虚拟节点是dropout节点时所需要的filter
        std::vector<Tensor*> m_input_data;
        std::unordered_map<std::string, Node*> m_op_node_map;// 缓存这个虚拟节点生成计算节点
        int m_share_parameter;
        float m_keep_rate;
        VirtualNode (std::string type, std::string id, int share_parameter = 0, float keep_rate = 0.5);
        void get_parents_op_nodes (int idx, Graph* compute_graph, std::vector<Node*> &node_list);

        // 根据虚拟节点的名字和内循环下标idx，确定生成的计算节点。如果在m_op_node_map中已经存在计算节点了则不重复生成计算节点，用于支持动态计算图
        Node* get_op_node (int idx);
        ~VirtualNode ();// 会释放该虚拟节点生成的计算节点的内存空间
};
#endif
