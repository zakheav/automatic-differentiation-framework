#ifndef ADD_H_
#define ADD_H_
#include "../OperatorNode.h"
#include <string>
class Add: public OperatorNode {
    public:
        Add (std::string type, std::string id, std::string idx);
        void op ();
        void grad_op ();
};
#endif
