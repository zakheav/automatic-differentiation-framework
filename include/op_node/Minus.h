#ifndef MINUS_H_
#define MINUS_H_
#include "../OperatorNode.h"
#include <string>
class Minus: public OperatorNode {
    public:
        Minus (std::string type, std::string id, std::string idx);
        void op ();
        void grad_op ();
};
#endif
