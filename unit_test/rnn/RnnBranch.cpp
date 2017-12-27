#include "RnnBranch.h"
#include "../../include/op_node/Input.h"
#include <sstream>
using namespace std;
RnnBranch::RnnBranch (string type, string id): BranchNode (type, id) {
}
Node* RnnBranch::choose_node (int idx, Graph* compute_graph) {
    ostringstream oss;
    if (idx == 0) {
        int batch_size = 1;
        vector<int> shape (2);
        shape[0]  = batch_size; shape[1] = 4;
        Tensor* init_tensor = new Tensor (shape);
        vector<Tensor*> data; data.push_back (init_tensor);
        Node* init_input = new Input ("Input", "init", "0", data);
        compute_graph -> add_node ("", init_input);
        ((Input*) init_input) -> op ();
        return init_input;
    } else {
        oss << (idx - 1) << ":";
        string name = "Mult:h:" + oss.str ();
        return compute_graph -> get_node (name);
    }
}
