#include "../include/Graph.h"
#include "../include/Node.h"
#include <vector>
#include <iostream>
using namespace std;
int main () {
    Node* a = new Node ("a", "0");
    Node* b = new Node ("b", "0");
    Node* c = new Node ("c", "0");
    Node* d = new Node ("d", "0");
    Node* e = new Node ("e", "0");
    Node* f = new Node ("f", "0");

    // 构建图
    Graph graph;
    graph.add_node ("", a);
    graph.add_node ("", b);
    graph.add_node (a -> get_name (), c);
    graph.add_node (b -> get_name (), c);
    graph.add_node ("", d);
    graph.add_node (c -> get_name (), e);
    graph.add_node (d -> get_name (), e);
    graph.add_node (c -> get_name (), f);
    // 构建转置图
    graph.build_reverse_graph ();    
    // 构建子图, 基于图改写
    vector<Node*> endnode_list;
    endnode_list.push_back (e);
    graph.build_subgraph (endnode_list);
    // 拓扑排序
    vector<Node*> toposort_result;
    graph.topological_sort (graph.m_adj_table, toposort_result);// 原图拓扑排序
    for (int i = 0; i < toposort_result.size (); ++i) {
        cout << toposort_result[i] -> get_name () << " ";
    }
    cout << endl;
    vector<Node*> reverse_toposort_result;
    graph.topological_sort (graph.m_reverse_table, reverse_toposort_result);// 转置图拓扑排序
    for (int i = 0; i < reverse_toposort_result.size (); ++i) {
        cout << reverse_toposort_result[i] -> get_name () << " ";
    }
    cout << endl;

}
