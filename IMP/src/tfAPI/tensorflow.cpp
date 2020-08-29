#include "tfAPI/tensorflow.hpp"

template<typename ValueType>
ConstantNode<ValueType>& TensorFlow::constant(NDArray<ValueType> &value, std::string dtype, Shape shape, std::string name) {
    ConstantNode<ValueType>* node = new ConstantNode<ValueType>(value, dtype, shape, name);
    dfg_.add_input_node(node);
    node->add_predecessors(dfg_.get_sources_node());
    return *node;
}
