#ifndef LOOPNODE_H_
#define LOOPNODE_H_
#include "Node.h"
#include "VirtualGraph.h"
class LoopNode: public Node {
    public:
        VirtualGraph* m_sub_vgraph;
        Node* m_end_compute_node;// 该循环节点最终的输出节点
        // 初始化循环，为循环子图中的一些节点补上节点依赖，依赖的节点来自于LoopNode所依赖的节点
        // 补上依赖的主要方式是把依赖的计算节点的名字加入到当前LoopNode中的子虚拟图的起始BranchNode中
        void (*init) (LoopNode* loop_node);
        int (*condition) (Graph* compute_graph, int idx);// 条件成立则返回1，否则返回0
        LoopNode (std::string type, std::string id, void (*func1) (LoopNode*), int (*func2) (Graph*, int));
        virtual void inner_loop (Graph* compute_graph);// 内循环
        virtual ~LoopNode ();
};
#endif
