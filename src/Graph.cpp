#include <iostream>
#include "../include/Graph.h"
using namespace std;
Graph::Graph () {
    m_need_build_reverse_graph_flag = 0;
}
void Graph::add_node (string parent_name, Node* node) {
    m_node_map[node -> get_name ()] = node;// 将节点加入字典
    if (parent_name != "") {
        if (m_node_map.end () != m_node_map.find (parent_name)) {
            Node* parent_node = m_node_map[parent_name];
            node -> m_parents.push_back (parent_node);
            m_adj_table[parent_name].push_back (node);// 节点加入邻接表
        } else {
            cout << "parent node is not in graph" << endl;
        }
    }
}
Node* Graph::get_node (string name) {
    if (m_node_map.end () == m_node_map.find (name)) {
        return 0;
    } else {
        return m_node_map[name];
    }
}
void Graph::build_subgraph (vector<Node*> &endnode_list) {
    unordered_map<string, Node*>::iterator node_map_it = m_node_map.begin ();
    while (node_map_it != m_node_map.end ()) {// 所有节点设置为不可见
        node_map_it -> second -> m_invisible = 1;
        ++node_map_it;
    }
    // 构造子图
    queue<Node*> q;
    unordered_set<Node*> visit;
    for (int i = 0; i < endnode_list.size (); ++i) {
        q.push (endnode_list[i]);
        visit.insert (endnode_list[i]);
    }
    while (!q.empty ()) {
        Node* node = q.front ();
        q.pop ();
        node -> m_invisible = 0;
        for (int i = 0; i < node -> m_parents.size (); ++i) {
            if (visit.find (node -> m_parents[i]) == visit.end ()) {
                visit.insert (node -> m_parents[i]);
                q.push (node -> m_parents[i]);
            }
        }
    }
}
void Graph::topological_sort (std::unordered_map<std::string, std::vector<Node*> > &adj_table, std::vector<Node*> &result) {
    unordered_map<string, int> indegree;
    unordered_map<string, Node*>::iterator node_map_it = m_node_map.begin ();
    while (node_map_it != m_node_map.end ()) {
        indegree[node_map_it -> first] = 0;
        ++node_map_it;
    }
    unordered_map<string, vector<Node*> >::iterator adj_table_it = adj_table.begin ();
    while (adj_table_it != adj_table.end ()) {
        for (int i = 0; i < adj_table_it -> second.size (); ++i) {
            ++indegree[(adj_table_it -> second)[i] -> get_name ()];
        }
        ++adj_table_it;
    }
    queue<Node*> q;
    unordered_map<string, int>::iterator indegree_it = indegree.begin ();
    while (indegree_it != indegree.end ()) {
        if (indegree_it -> second == 0) {
            q.push (m_node_map[indegree_it -> first]);
        }
        ++indegree_it;
    }
    while (!q.empty ()) {
        Node* node = q.front ();
        q.pop ();
        if (node -> m_invisible == 0) {// 可见节点加入result
            result.push_back (node);
        }
        vector<Node*> adj_nodes = adj_table[node -> get_name ()];
        for (int i = 0; i < adj_nodes.size (); ++i) {
            --indegree[(adj_nodes[i]) -> get_name ()];
            if (indegree[(adj_nodes[i]) -> get_name ()] == 0) {
                q.push (adj_nodes[i]);
            }
        }
    }
}
void Graph::build_reverse_graph () {
    unordered_map<string, vector<Node*> >::iterator adj_table_it = m_adj_table.begin ();
    while (adj_table_it != m_adj_table.end ()) {
        Node* parent = m_node_map[adj_table_it -> first];
        vector<Node*> adj_nodes = adj_table_it -> second;
        for (int i = 0; i < adj_nodes.size (); ++i) {
            string name = (adj_nodes[i]) -> get_name ();
            m_reverse_table[name].push_back (parent);
        }
        ++adj_table_it;
    }
    m_need_build_reverse_graph_flag = 1;
}
void Graph::get_endnode (vector<Node*> &endnode_list) {
    if (m_need_build_reverse_graph_flag == 0) {// 没有构建转置图
        build_reverse_graph ();
    }
    unordered_map<string, vector<Node*> >::iterator reverse_table_it = m_reverse_table.begin ();
    unordered_map<string, Node*>::iterator node_map_it = m_node_map.begin ();
    unordered_map<string, int> indegree;
    while (node_map_it != m_node_map.end ()) {
        indegree[node_map_it -> first] = 0;
        ++node_map_it;
    }
    while (reverse_table_it != m_reverse_table.end ()) {
        vector<Node*> adj_nodes = reverse_table_it -> second;
        for (int i = 0; i < adj_nodes.size (); ++i) {
            ++indegree[(adj_nodes[i]) -> get_name ()];
        }
        ++reverse_table_it;
    }
    unordered_map<string, int>::iterator indegree_it = indegree.begin ();
    while (indegree_it != indegree.end ()) {
        if (indegree_it -> second == 0) {
            endnode_list.push_back (m_node_map[indegree_it -> first]);
        }
        ++indegree_it;
    }
}
Graph::~Graph () {
    cout << "free node_map" << endl;
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
