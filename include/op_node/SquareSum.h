#ifndef SQUARESUM_H_
#define SQUARESUM_H_
#include "../OperatorNode.h"
class SquareSum: public OperatorNode {
    public:
        SquareSum (std::string type, std::string id, std::string idx);
        void op ();
        void grad_op ();
        ~SquareSum ();
        void release_tensor ();
};
#endif
