#ifndef OPERATORNODE_H_
#define OPERATORNODE_H_
#include "Node.h"
#include "Tensor.h"
#include <string>
class OperatorNode: public Node {
    private:
        float m_a;
    protected:
        void chain_rule (Tensor* grad, int parent_idx);
    public:
        Tensor* m_output;
        Tensor* m_sum_grad;
        OperatorNode (std::string type, std::string id, std::string idx);
        virtual void op ();
        virtual void grad_op ();
        void update ();
        virtual ~OperatorNode ();
        virtual void release_tensor ();
};
#endif
