#include "RnnLoop.h"
using namespace std;
RnnLoop::RnnLoop (string type, string id): LoopNode (type, id) {
}
int RnnLoop::condition (Graph* compute_graph, int idx) {
    if (idx < 8) {
        return 0;
    } else {
        return 1;
    }
}
