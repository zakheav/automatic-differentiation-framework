#ifndef RNNLOOP_H_
#define RNNLOOP_H_
#include "../../include/LoopNode.h"
class RnnLoop: public LoopNode {
    public:
        RnnLoop (std::string type, std::string id);
        int condition (Graph* compute_graph, int idx);
};
#endif
