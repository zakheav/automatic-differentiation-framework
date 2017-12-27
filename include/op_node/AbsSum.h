#ifndef ABSSUM_H_
#define ABSSUM_H_
#include "../OperatorNode.h"
class AbsSum: public OperatorNode {
    public:
        AbsSum (std::string type, std::string id, std::string idx);
        void op ();
        void grad_op ();
        ~AbsSum ();
        void release_tensor ();
};
#endif
