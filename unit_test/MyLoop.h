#ifndef MYLOOP_H_
#define MYLOOP_H_
#include "../include/LoopNode.h"
class MyLoop: public LoopNode {
    public:
        MyLoop (std::string type, std::string id);
        int condition (Graph* compute_graph, int idx);
        void init (Graph* compute_graph);
};
#endif
