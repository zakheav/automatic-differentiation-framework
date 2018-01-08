#include "../include/VirtualNode.h"
#include "../include/BranchNode.h"
#include "../include/LoopNode.h"
#include "../include/op_node/Add.h"
#include "../include/op_node/Input.h"
#include "../include/op_node/Mult.h"
#include "../include/op_node/Bias.h"
#include "../include/op_node/Minus.h"
#include "../include/op_node/SquareSum.h"
#include "../include/op_node/AbsSum.h"
#include "../include/op_node/Sigmoid.h"
#include "../include/op_node/Parameter.h"
#include "../include/op_node/RnnInputX.h"
#include "../include/op_node/RnnInputY.h"
#include <sstream>
#include <iostream>
using namespace std;
VirtualNode::VirtualNode (string type, string id, int share_parameter): Node (type, id) {
    m_share_parameter = share_parameter;
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
Node* VirtualNode::get_op_node (int idx) {// 一个OperatorNode工厂
    ostringstream oss;
    oss << idx;
    Node* op_node = 0;
    string op_node_name = m_name[0] + ":" + m_name[1] + ":" + oss.str () + ":";
    if (m_op_node_map.find (op_node_name) == m_op_node_map.end ()) {// 之前没生成过该计算节点
        if (m_name[0] == "Add") {
            op_node = new Add (m_name[0], m_name[1], oss.str ());
        } else if (m_name[0] == "Input") {
            if (m_input_data.size () == 0) {
                cout << "input data is not initialize" << endl;
            } else {
                op_node = new Input (m_name[0], m_name[1], oss.str (), m_input_data);
            }
        } else if (m_name[0] == "Parameter") {
            if (m_data == 0) {
                cout << "parameter node is not initialize" << endl;
            } else {
                op_node = new Parameter (m_name[0], m_name[1], oss.str (), m_data, m_share_parameter);
            }
        } else if (m_name[0] == "SquareSum") {
            op_node = new SquareSum (m_name[0], m_name[1], oss.str ());
        } else if (m_name[0] == "AbsSum") {
            op_node = new AbsSum (m_name[0], m_name[1], oss.str ());
        } else if (m_name[0] == "Mult") {
            op_node = new Mult (m_name[0], m_name[1], oss.str ());
        } else if (m_name[0] == "Minus") {
            op_node = new Minus (m_name[0], m_name[1], oss.str ());
        } else if (m_name[0] == "Sigmoid") {
            op_node = new Sigmoid (m_name[0], m_name[1], oss.str ());
        } else if (m_name[0] == "Bias") {
            op_node =  new Bias (m_name[0], m_name[1], oss.str ());
        } else if (m_name[0] == "RnnInputX") {
            if (m_input_data.size () == 0) {
                cout << "input data is not initialize" << endl;
            } else {
                op_node = new RnnInputX (m_name[0], m_name[1], oss.str (), m_input_data);
            }
        } else if (m_name[0] == "RnnInputY") {
            if (m_input_data.size () == 0) {
                cout << "input data is not initialize" << endl;
            } else {
                op_node = new RnnInputY (m_name[0], m_name[1], oss.str (), m_input_data);
            }
        } else {
            cout << "op node name error" << endl;
        }
        m_op_node_map[op_node_name] = op_node;
    } else {// 直接找到虚拟节点生成过的该计算节点
        op_node = m_op_node_map[op_node_name];
    }
    return op_node;
}
VirtualNode::~VirtualNode () {
    // cout << "free virtualNode: " << get_name () << endl;
    if (m_data != 0) {
        delete m_data;
    }
    for (int i = 0; i < m_input_data.size (); ++i) {
        delete m_input_data[i];
    }
    vector<Tensor*> ().swap (m_input_data);
    // 释放每个虚拟节点生成的计算节点
    unordered_map<string, Node*>::iterator op_node_map_it = m_op_node_map.begin ();
    while (op_node_map_it != m_op_node_map.end ()) {
        delete op_node_map_it -> second;
        ++op_node_map_it;
    }
    m_op_node_map.clear ();
}
