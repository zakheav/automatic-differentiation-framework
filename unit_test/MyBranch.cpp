#include "MyBranch.h"
#include <sstream>
using namespace std;
MyBranch::MyBranch (string type, string id): BranchNode (type, id) {
    m_parent_compute_node = 0;
}
Node* MyBranch::choose_node (int idx, Graph* compute_graph) {
    if (idx == 0) {
        return m_parent_compute_node;
    } else {
        ostringstream oss;
        oss << (idx - 1) << ":";
        string name = "SquareSum:1:" + oss.str ();
        return compute_graph -> get_node (name);
    }
}
