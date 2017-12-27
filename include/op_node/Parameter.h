#ifndef PARAMETER_H_
#define PARAMETER_H_
#include "../OperatorNode.h"
#include <string>
class Parameter: public OperatorNode {
    public:
        int m_share_data;
        Parameter (std::string type, std::string id, std::string idx, Tensor* data, int share_data = 0);
        ~Parameter ();
        void release_tensor ();
};
#endif
