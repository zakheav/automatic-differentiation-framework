#ifndef OPTIMIZER_H_
#define OPTIMIZER_H_
#include <vector>
#include "Node.h"
class Optimizer {
    protected:
        float m_a;
    public:
        Optimizer (float a);
        virtual void optimize (std::vector<Node*> &topo_results);
        virtual ~Optimizer ();
};
#endif
