#include "../include/Node.h"
using namespace std;
Node::Node (string type, string id) {
    m_name.push_back (type);
    m_name.push_back (id);
    m_invisible = 0;
}
std::string Node::get_name () {
    string name = "";
    for (int i = 0; i < m_name.size (); ++i) {
        name += m_name[i] + ":";
    }
    return name;
}
Node::~Node () {
}
