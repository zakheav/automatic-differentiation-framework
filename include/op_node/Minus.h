#ifndef MINUS_H_
#define MINUS_H_
#include "../OperatorNode.h"
class Minus: public OperatorNode {
    public:
        Minus (std::string type, std::string id, std::string idx);
        void op ();
        void grad_op ();
        ~Minus ();
        void release_tensor ();
};
#endif
