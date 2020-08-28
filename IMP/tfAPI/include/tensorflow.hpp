#include "op_node.h"
#include "input_node.h"
#include "dfg.hpp"

class TFMath {
public:
    AbsNode abs(Tensor x, std::string name = "");
    AddNode add(Tensor x, Tensor y, std::string name = "");
};

class TensorFlow {
public:
    TFMath math;
    DFG& dfg_;

    template<typename ValueType>
    VariableNode<ValueType> Variable(NDArray<ValueType> &initial_value, Type dtype, Shape shape, std::string name = "");

    template<typename ValueType>
    ConstantNode<ValueType> constant(NDArray<ValueType> &value, Type dtype = NULL, Shape shape = NULL, std::string name = "Const");    

    PlaceholderNode placeholder(Type dtype, Shape shape, std::string name = "");

    void run();
};