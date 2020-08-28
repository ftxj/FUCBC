#include "tensorflow.hpp"

template<typename ValueType>
ConstantNode<ValueType>& TensorFlow::constant(NDArray<ValueType> &value, std::string dtype, Shape shape, std::string name = "Const") {
    ConstantNode<ValueType>* node = new ConstNode<ValueType>(value, dtype, shape, name);
    dfg_.add_input_node(node);
    node->add_predecessors(dfg_.get_sources_node());
    return *node;
}
