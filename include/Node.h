#ifndef NODE_H_
#define NODE_H_
#include <string>
#include <vector>
class Node {
    public:
        std::vector<std::string> m_name;
        std::vector<Node*> m_parents;// 依赖节点列表
        int m_invisible;// 0可见，1不可见
        
        virtual std::string get_name ();
        Node (std::string type, std::string id);
        virtual ~Node ();
};
#endif
