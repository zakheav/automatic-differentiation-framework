#include "MyLoop.h"
#include "MyBranch.h"
using namespace std;
MyLoop::MyLoop (string type, string id): LoopNode (type, id) {
}
void MyLoop::init (Graph* compute_graph) {
    ((MyBranch*) (m_sub_vgraph -> get_node ("Branch:1:"))) -> m_parent_compute_node = compute_graph -> get_node ("Parameter:1:0:");
}
int MyLoop::condition (Graph* compute_graph, int idx) {
    if (idx >= 4) {
        return 1;
    } else {
        return 0;
    }
}
