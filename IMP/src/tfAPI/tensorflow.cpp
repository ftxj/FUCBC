#include "tfAPI/tensorflow.hpp"

TensorFlow::TensorFlow() {
    print = new Print();
    dfg_ = new DFG();
}

ConstantNode* TensorFlow::constant(NDArray value, std::string dtype, Shape shape, std::string name) {
    ConstantNode* node = new ConstantNode(value, dtype, shape, name);
    node->set_dfg(dfg_);
    dfg_->add_input_node(node);
    return node;
}


void TensorFlow::run() {
    dfg_->BFS(print);
}


std::string TFMath::dtype_check(Tensor *x, Tensor *y) {
    // TODO
    return x->get_dtype();
}

Shape TFMath::shape_check(Tensor *x, Tensor *y) {
    // TODO
    return x->get_shape();
}

DFG* TFMath::dfg_check(BaseNode *x, BaseNode *y) {
    // TODO
    return x->get_dfg();
}

AddNode* TFMath::add(Tensor *x, Tensor *y, std::string name) {
    std::string dtype = dtype_check(x, y);
    Shape shape = shape_check(x, y);
    BaseNode* a = static_cast<BaseNode*>(x);
    BaseNode* b = static_cast<BaseNode*>(y);
    DFG* dfg = dfg_check(a, b);

    if(a == nullptr || b == nullptr) {
        assert(0);
    }
    AddNode* node = new AddNode(a, b, name, shape, dtype);
    node->set_dfg(dfg);
    dfg->add_node(node, {a, b});
    return node;
}