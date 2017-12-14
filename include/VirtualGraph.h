#ifndef VIRTUALGRAPH_H_
#define VIRTUALGRAPH_H_
#include "Graph.h"
#include "ComputeGraph.h"
class VirtualGraph: public Graph {
    public:
        Node* build_compute_graph (Graph* compute_graph, int idx = 0);
        ~VirtualGraph ();
};
#endif
