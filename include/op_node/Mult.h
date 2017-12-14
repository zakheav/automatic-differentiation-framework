#ifndef MULT_H_
#define MULT_H_
#include "../OperatorNode.h"
#include <string>
class Mult: public OperatorNode {
    public:
        Mult (std::string type, std::string id, std::string idx);
        void op ();
        void grad_op ();
};
#endif
