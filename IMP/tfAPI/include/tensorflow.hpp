#include "op_node.h"
#include "input_node.h"

class TFMath {
public:
    AbsNode abs(Tensor x, std::string name = "");
    AddNode add(Tensor x, Tensor y, std::string name = "");
};

class TensorFlow {
public:
    TFMath math;
    template<typename ValueType>
    VariableNode<ValueType> Variable(ValueType initial_value, Type dtype, Shape shape, std::string name = "");

    template<typename ValueType>
    ConstNode<ValueType> constant(ValueType initial_value, Type dtype, Shape shape, std::string name = "Const");    

    PlaceholderNode placeholder(Type dtype, Shape shape, std::string name = "");
};