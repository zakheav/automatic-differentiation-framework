#ifndef BIAS_H_
#define BIAS_H_
#include "../OperatorNode.h"
#include <string>
class Bias: public OperatorNode {
    public:
        Bias (std::string type, std::string id, std::string idx);
        void op ();
        void grad_op ();
};
#endif
