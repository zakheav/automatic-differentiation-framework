#ifndef PARAMETER_H_
#define PARAMETER_H_
#include "../OperatorNode.h"
#include <string>
class Parameter: public OperatorNode {
    public:
        Parameter (std::string type, std::string id, std::string idx, Tensor* data);
};
#endif
