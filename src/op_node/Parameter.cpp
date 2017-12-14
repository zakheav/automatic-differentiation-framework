#include "../../include/op_node/Parameter.h"
using namespace std;
Parameter::Parameter (string type, string id, string idx, Tensor* data): OperatorNode (type, id, idx) {
    m_output = data;
}
