#include "../include/VirtualNode.h"
#include "../include/BranchNode.h"
#include "../include/LoopNode.h"
#include "../include/op_node/Add.h"
#include "../include/op_node/Input.h"
#include "../include/op_node/Mult.h"
#include "../include/op_node/Minus.h"
#include "../include/op_node/SquareSum.h"
#include "../include/op_node/Sigmoid.h"
#include "../include/op_node/Parameter.h"
#include <sstream>
#include <iostream>
using namespace std;
VirtualNode::VirtualNode (string type, string id): Node (type, id) {
}
void VirtualNode::get_parents_op_nodes (int idx, Graph* compute_graph, vector<Node*> &node_list) {
    ostringstream oss;
    oss << idx << ":";
    for (int i = 0; i < m_parents.size (); ++i) {
        if (m_parents[i] -> m_name[0] == "Branch") {
            node_list.push_back (((BranchNode*) m_parents[i]) -> choose_node (idx, compute_graph));
        } else if (m_parents[i] -> m_name[0] == "Loop") {
            node_list.push_back (((LoopNode*) m_parents[i]) -> m_end_compute_node);
        } else {
            string op_node_name = m_parents[i] -> get_name () + oss.str ();
            node_list.push_back (compute_graph -> get_node (op_node_name));
        }
        
    }
}
Node* VirtualNode::get_op_node (int idx) {
    ostringstream oss;
    oss << idx;
    if (m_name[0] == "Add") {
        return new Add (m_name[0], m_name[1], oss.str ());
    } else if (m_name[0] == "Input") {
        if (m_input_data.size () == 0) {
            cout << "input data is not initialize" << endl;
            return 0;
        } else {
            return new Input (m_name[0], m_name[1], oss.str (), m_input_data);
        }
    } else if (m_name[0] == "Parameter") {
        if (m_data == 0) {
            cout << "parameter node is not initialize" << endl;
            return 0;
        } else {
            return new Parameter (m_name[0], m_name[1], oss.str (), m_data);
        }
    } else if (m_name[0] == "SquareSum") {
        return new SquareSum (m_name[0], m_name[1], oss.str ());
    } else if (m_name[0] == "Mult") {
        return new Mult (m_name[0], m_name[1], oss.str ());
    } else if (m_name[0] == "Minus") {
        return new Minus (m_name[0], m_name[1], oss.str ());
    } else if (m_name[0] == "Sigmoid") {
        return new Sigmoid (m_name[0], m_name[1], oss.str ());
    } else if (m_name[0] == "Bias") {
        return new Bias (m_name[0], m_name[1], oss.str ());
    } else {
        return 0;
    }
}
VirtualNode::~VirtualNode () {
    // cout << "free virtualNode: " << get_name () << endl;
    if (m_data != 0) {
        delete m_data;
    }
    for (int i = 0; i < m_input_data.size (); ++i) {
        delete m_input_data[i];
    }
}
