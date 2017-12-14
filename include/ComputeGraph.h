#ifndef COMPUTEGRAPH_H_
#define COMPUTEGRAPH_H_
#include "Graph.h"
#include "Tensor.h"
#include <vector>
class ComputeGraph: public Graph {
    public:
        void forward_propagation (std::vector<Tensor*> &result_list);
        void back_propagation ();
        void release_tensor ();
        ~ComputeGraph ();
};
#endif
