#ifndef INPUT_H_
#define INPUT_H_
#include "../OperatorNode.h"
#include <vector>
class Input: public OperatorNode {
    public:
        int m_data_ptr;
        std::vector<Tensor*> m_data;
        Input (std::string type, std::string id, std::string idx, std::vector<Tensor*> input_data, void (*func) (Input*) = 0);
        void (*op) (Input* input);// 参数是该函数所在对象本身
        void release_tensor ();
        ~Input ();
};
#endif
