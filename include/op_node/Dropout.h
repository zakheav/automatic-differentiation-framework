#ifndef DROPOUT_H_
#define DROPOUT_H_
#include "../OperatorNode.h"
class Dropout: public OperatorNode {
    public:
        float m_keep_rate;
        Tensor* m_filter;
        Dropout (std::string type, std::string id, std::string idx, Tensor* filter, float keep_rate);
        void op ();
        void grad_op ();
        ~Dropout ();
        void release_tensor ();
};
#endif
