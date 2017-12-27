#ifndef RNNINPUTY_H_
#define RNNINPUTY_H_
#include "../OperatorNode.h"
#include <vector>
class RnnInputY: public OperatorNode {
    public:
        int m_data_ptr;
        std::vector<Tensor*> m_data;
        RnnInputY (std::string type, std::string id, std::string idx, std::vector<Tensor*> input_data);
        void op ();
        ~RnnInputY ();
        void release_tensor ();
};
#endif
