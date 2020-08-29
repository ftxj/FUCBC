#include "tfnode/op_node.hpp"
#include "tfnode/input_node.hpp"
#include "tfnode/dfg.hpp"

class TFMath {
public:
    // AbsNode abs(Tensor x, std::string name = "");
    
    template<typename T1>
    AddNode<T1> &add(Tensor<T1> x, Tensor<T1> y, std::string name = "");
};

class TensorFlow {
public:
    TFMath math;
    DFG& dfg_;

    //template<typename ValueType>
    //VariableNode<ValueType> Variable(NDArray<ValueType> &initial_value, Type dtype, Shape shape, std::string name = "");

    template<typename ValueType>
    ConstantNode<ValueType>& constant(NDArray<ValueType> &value, std::string dtype = "unknow", Shape shape = Shape(), std::string name = "Const");    

    //PlaceholderNode placeholder(Type dtype, Shape shape, std::string name = "");

    void run();
};