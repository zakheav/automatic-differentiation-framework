#ifndef SIGMOID_H_
#define SIGMOID_H_
#include "../OperatorNode.h"
#include <string>
class Sigmoid: public OperatorNode {
    public:
        Sigmoid (std::string type, std::string id, std::string idx);
        void op ();
        void grad_op ();
};
#endif
