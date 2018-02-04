#include "../include/VirtualGraph.h"
#include "../include/VirtualNode.h"
#include "../include/LoopNode.h"
#include "../include/OperatorNode.h"
#include <unordered_map>
#include <iostream>
using namespace std;
Node* VirtualGraph::build_compute_graph (Graph* compute_graph, int idx) {// 输入计算图的引用
    vector<Node*> topo_result;
    topological_sort (m_adj_table, topo_result);
    Node* end_node = 0;
    for (int i = 0; i < topo_result.size (); ++i) {// 构建
        // cout << topo_result[i] -> get_name () << endl;
        if (topo_result[i] -> m_name[0] == "Loop") {// 如果是循环节点
            LoopNode* loop_node = (LoopNode*) topo_result[i];
            loop_node -> inner_loop (compute_graph);// 执行循环
            end_node = loop_node -> m_end_compute_node;
        } else if (topo_result[i] -> m_name[0] == "Branch") {// 如果是分支结点
            // do nothing
        } else {// 普通虚拟节点
            VirtualNode* v_node = (VirtualNode*) topo_result[i];
            if (v_node -> m_parents.size () == 0) {// 该虚拟节点没有依赖的虚拟节点
                Node* op_node = v_node -> get_op_node (idx);
                compute_graph -> add_node ("", op_node);// 向计算图中添加节点
                ((OperatorNode*) op_node) -> op ();// 执行该计算节点
                end_node = op_node;
            } else {
                vector<Node*> parents_op_node;
                v_node -> get_parents_op_nodes (idx, compute_graph, parents_op_node);
                int need_op_node = 1;
                for (int i = 0; i < parents_op_node.size (); ++i) {
                    if (parents_op_node[i] == 0) {// 存在没有生成的依赖的计算节点
                        need_op_node = 0;
                    }
                }
                if (need_op_node == 1) {// 当前计算节点可以生成
                    Node* op_node = v_node -> get_op_node (idx);
                    for (int i = 0; i < parents_op_node.size (); ++i) {
                        compute_graph -> add_node (parents_op_node[i] -> get_name (), op_node);
                    }
                    ((OperatorNode*) op_node) -> op ();// 执行该计算节点
                    end_node = op_node;
                }
            }
        }
    }
    return end_node;
}
VirtualGraph::~VirtualGraph () {
    cout << "virtual graph free" << endl;
    // 释放虚拟节点
    cout << "virtual node_map free" << endl;
    unordered_map<string, Node*>::iterator node_map_it = m_node_map.begin ();
    while (node_map_it != m_node_map.end ()) {
        delete node_map_it -> second;
        node_map_it -> second = 0;
        ++node_map_it;
    }
    m_node_map.clear ();
    m_adj_table.clear ();
    m_reverse_table.clear ();
}
