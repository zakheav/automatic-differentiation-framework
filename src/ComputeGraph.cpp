#include "../include/ComputeGraph.h"
#include "../include/OperatorNode.h"
#include "../include/op_node/Input.h"
#include <iostream>
using namespace std;
ComputeGraph::ComputeGraph () {
    m_need_release_tensor_flag = 0;
}
void ComputeGraph::forward_propagation (vector<Node*> &result_list) {
    if (m_need_release_tensor_flag == 1) {// 前向传播前释放上一次的运算结果
        release_tensor ();
    }
    vector<Node*> topo_result;
    topological_sort (m_adj_table, topo_result);
    for (int i = 0; i < topo_result.size (); ++i) {
        if (topo_result[i] -> m_name[0] == "Input") {
            ((Input*) topo_result[i]) -> op ((Input*) topo_result[i]);
        } else {
            ((OperatorNode*) topo_result[i]) -> op ();
        }
    }
    get_endnode (result_list);
    m_need_release_tensor_flag = 1;
}
void ComputeGraph::back_propagation () {
    if (m_need_build_reverse_graph_flag == 1) {
        build_reverse_graph ();
    }
    vector<Node*> topo_result;
    topological_sort (m_reverse_table, topo_result);
    if (m_optimizer == 0) {
        cout << "optimizer has not been set" << endl;
    } else {
        m_optimizer -> optimize (topo_result);
    }
    m_need_release_tensor_flag = 1;
}
void ComputeGraph::release_tensor () {
    unordered_map<string, Node*>::iterator node_map_it = m_node_map.begin ();
    while (node_map_it != m_node_map.end ()) {
        ((OperatorNode*) (node_map_it -> second)) -> release_tensor ();
        ++node_map_it;
    }
    m_need_release_tensor_flag = 0;
}
ComputeGraph::~ComputeGraph () {
    cout << "compute graph free" << endl;
    delete m_optimizer;
    m_optimizer = 0;
    m_node_map.clear ();
    m_adj_table.clear ();
    m_reverse_table.clear ();
}
