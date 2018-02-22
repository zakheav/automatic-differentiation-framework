#include "../include/LoopNode.h"
#include <iostream>
using namespace std;
LoopNode::LoopNode (string type, string id, void (*func1) (LoopNode*), int (*func2) (Graph*, int)): Node (type, id) {
    m_sub_vgraph = new VirtualGraph ();
    m_end_compute_node = 0;
    init = func1;
    condition = func2;
}
void LoopNode::inner_loop (Graph* compute_graph) {
    int idx = 0;
    init (this);// 初始化循环
    while (condition (compute_graph, idx) == 0) {
        m_end_compute_node = m_sub_vgraph -> build_compute_graph (compute_graph, idx);
        ++idx;
    }
}
LoopNode::~LoopNode () {
    cout << "free LoopNode: " << get_name () << endl;
    delete m_sub_vgraph;
    m_sub_vgraph = 0;
    m_end_compute_node = 0;
}
