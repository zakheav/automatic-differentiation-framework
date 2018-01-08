#ifndef ADADELTA_H_
#define ADADELTA_H_
#include "../Optimizer.h"
#include "../Tensor.h"
#include <unordered_map>
class Adadelta: public Optimizer {
    private:
        std::unordered_map<std::string, Tensor*> m_tensor_store;
        float m_epsl;
        float m_lambda;
    public:
        Adadelta (float a);
        void optimize (std::vector<Node*> &topo_results);
        ~Adadelta ();
};
#endif
