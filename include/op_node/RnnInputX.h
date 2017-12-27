#ifndef RNNINPUTX_H_
#define RNNINPUTX_H_
#include "../OperatorNode.h"
#include <vector>
class RnnInputX: public OperatorNode {
    public:
        int m_data_ptr;
        std::vector<Tensor*> m_data;
        RnnInputX (std::string type, std::string id, std::string idx, std::vector<Tensor*> input_data);
        void op ();
        ~RnnInputX ();
        void release_tensor ();
};
#endif
