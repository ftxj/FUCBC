#include "tfAPI/tensorflow.hpp"

TensorFlow::TensorFlow() {
    print = new Print();
    dfg_ = new DFG();
}

ConstantNode* TensorFlow::constant(NDArray value, std::string dtype, Shape shape, std::string name) {
    ConstantNode* node = new ConstantNode(value, dtype, shape, name);
    dfg_->add_nodes(node);
    node->set_dfg(dfg_);

    dfg_->add_input_node(node);
    return node;
}


void TensorFlow::run() {
    dfg_->BFS(print);
}