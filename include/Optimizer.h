#ifndef OPTIMIZER_H_
#define OPTIMIZER_H_
#include <vector>
#include "Node.h"
class Optimizer {
    public:
        float m_a;
        Optimizer (float a);
        void optimize (std::vector<Node*> &topo_results);
        virtual ~Optimizer ();
};
#endif
