#include "tfAPI/tensorflow.hpp"
#include "data_structure/shape.hpp"

TensorFlow::TensorFlow() : math(TFMath()) {
    print = new Print();
    dfg_ = new DFG();
    math.set_dfg(dfg_);
}

ConstantNode* TensorFlow::constant(NDArray value, std::string dtype, Shape shape, std::string name) {
    ConstantNode* node = new ConstantNode(value, dtype, shape, name);
    node->set_dfg(dfg_);
    dfg_->add_input_node(node);
    return node;
}

PlaceholderNode* TensorFlow::placeholder(std::string dtype, Shape shape, std::string name) {
    PlaceholderNode* node = new PlaceholderNode(dtype, shape, name);
    node->set_dfg(dfg_);
    dfg_->add_input_node(node);
    return node;
} 
VariableNode* TensorFlow::Variable(NDArray value, std::string dtype, Shape shape, std::string name) {
    VariableNode* node = new VariableNode(value, dtype, shape, name);
    node->set_dfg(dfg_);
    dfg_->add_input_node(node);
    return node;
}

void TensorFlow::run() {
    dfg_->topological_pass(print);
}

std::string TFMath::dtype_check(Tensor *x, Tensor *y) {
    // TODO
    return x->get_dtype();
}

Shape TFMath::shape_check(Tensor *x, Tensor *y) {
    // TODO
    return x->get_shape();
}

std::pair<BaseNode*, BaseNode*> TFMath::element_wise_shape_transfer(Tensor *x, Tensor *y) {
    BaseNode* a = static_cast<BaseNode*>(x);
    BaseNode* b = static_cast<BaseNode*>(y);
    if(a == nullptr || b == nullptr) {
        assert_msg(0, "static_cast<> fail: x=" << x << ",y=" << y);
    }
    if(a->get_shape() > b->get_shape()) {
        ReShapeNode* node = new ReShapeNode(b, "reshap", a->get_shape(), dtype_check(x, y));
        dfg_->add_node(node, {b});
        return {a, node};
    }
    else if(a->get_shape() < b->get_shape()) {
        ReShapeNode* node = new ReShapeNode(a, "reshap", b->get_shape(), dtype_check(x, y));
        dfg_->add_node(node, {a});
        return {node, b};
    }
    else {
        return {a, b};
    }
}

AbsNode* TFMath::abs(Tensor *x, std::string name) {
    BaseNode* a = static_cast<BaseNode*>(x);
    AbsNode* node = new AbsNode(a, name, a->get_shape(), a->get_dtype());
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a});
    return node;
}

ArgMinNode* TFMath::argmin(Tensor *x, int axis, std::string output_type, std::string name) {
    BaseNode* a = static_cast<BaseNode*>(x);
    ArgMinNode* node = new ArgMinNode(a, axis, name, a->get_shape(), a->get_dtype());
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a});
    return node;
}

ExpNode* TFMath::exp(Tensor *x, std::string name) {
    BaseNode* a = static_cast<BaseNode*>(x);
    ExpNode* node = new ExpNode(a, name, a->get_shape(), a->get_dtype());
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a});
    return node;
}

ExpandDimsNode* TFMath::expand_dims(Tensor *x, int axis, std::string name) {
    BaseNode* a = static_cast<BaseNode*>(x);
    ExpandDimsNode* node = new ExpandDimsNode(a, axis, name, a->get_shape(), a->get_dtype());
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a});
    return node;
}

ReShapeNode* TFMath::reshape(Tensor *x, Shape shape, std::string name) {
    BaseNode* a = static_cast<BaseNode*>(x);
    ReShapeNode* node = new ReShapeNode(a, name, a->get_shape(), a->get_dtype());
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a});
    return node;
}

SigmoidNode* TFMath::sigmod(Tensor *x, std::string name) {
    BaseNode* a = static_cast<BaseNode*>(x);
    SigmoidNode* node = new SigmoidNode(a, name, a->get_shape(), a->get_dtype());
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a});
    return node;
}

SqrtNode* TFMath::sqrt(Tensor *x, std::string name) {
    BaseNode* a = static_cast<BaseNode*>(x);
    SqrtNode* node = new SqrtNode(a, name, a->get_shape(), a->get_dtype());
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a});
    return node;
}

SquareNode* TFMath::square(Tensor *x, std::string name) {
    BaseNode* a = static_cast<BaseNode*>(x);
    SquareNode* node = new SquareNode(a, name, a->get_shape(), a->get_dtype());
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a});
    return node;
}

AddNode* TFMath::add(Tensor *x, Tensor *y, std::string name) {
    std::string dtype = dtype_check(x, y);
    auto real_shape_nodes = element_wise_shape_transfer(x, y);
    BaseNode* a = real_shape_nodes.first;
    BaseNode* b = real_shape_nodes.second;
    AddNode* node = new AddNode(a, b, name, a->get_shape(), dtype);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a, b});
    return node;
}

MatMulNode* TFMath::matmul(Tensor *x, Tensor *y, 
        bool transpose_a, bool transpose_b,
        bool a_is_sparse, bool b_is_sparse,
        std::string name
    ) {
    std::string dtype = dtype_check(x, y);
    //TODO shape check
    BaseNode* a = static_cast<BaseNode*>(x);
    BaseNode* b = static_cast<BaseNode*>(y);
    if(transpose_a) {
        TransposeNode* trans_a_node = new TransposeNode(a, "Transpose1", a->get_shape(), a->get_dtype());
        dfg_->add_node(trans_a_node, {a});
        a = trans_a_node;
    }
    if(transpose_b) {
        TransposeNode* trans_b_node = new TransposeNode(b, "Transpose2", b->get_shape(), b->get_dtype()); 
        dfg_->add_node(trans_b_node, {b});
        b = trans_b_node;
    }
    Shape shape = Shape::get_matmul_shape(a->get_shape(), b->get_shape());
    MatMulNode* node = new MatMulNode(a, b, name, shape, dtype);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a, b});
    return node;
}

TensordotNode* TFMath::tensordot(Tensor *x, Tensor *y, std::vector<int> axes,  std::string name) {
    // TODO rewrite this bug 
    std::string dtype = dtype_check(x, y);
    auto real_shape_nodes = element_wise_shape_transfer(x, y);
    BaseNode* a = real_shape_nodes.first;
    BaseNode* b = real_shape_nodes.second;
    TensordotNode* node = new TensordotNode(a, b, name, a->get_shape(), dtype);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a, b});
    return node;
}

DivNode* TFMath::div(Tensor *x, Tensor *y, std::string name) {
    std::string dtype = dtype_check(x, y);
    auto real_shape_nodes = element_wise_shape_transfer(x, y);
    BaseNode* a = real_shape_nodes.first;
    BaseNode* b = real_shape_nodes.second;
    DivNode* node = new DivNode(a, b, name, a->get_shape(), dtype);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a, b});
    return node;
}

FloorDivNode* TFMath::floordiv(Tensor *x, Tensor *y, std::string name) {
    std::string dtype = dtype_check(x, y);
    auto real_shape_nodes = element_wise_shape_transfer(x, y);
    BaseNode* a = real_shape_nodes.first;
    BaseNode* b = real_shape_nodes.second;
    FloorDivNode* node = new FloorDivNode(a, b, name, a->get_shape(), dtype);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a, b});
    return node;
}

LessNode* TFMath::less(Tensor *x, Tensor *y, std::string name) {
    std::string dtype = dtype_check(x, y);
    auto real_shape_nodes = element_wise_shape_transfer(x, y);
    BaseNode* a = real_shape_nodes.first;
    BaseNode* b = real_shape_nodes.second;
    LessNode* node = new LessNode(a, b, name, a->get_shape(), dtype);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a, b});
    return node;
}

MulNode* TFMath::multiply(Tensor *x, Tensor *y, std::string name) {
    std::string dtype = dtype_check(x, y);
    auto real_shape_nodes = element_wise_shape_transfer(x, y);
    BaseNode* a = real_shape_nodes.first;
    BaseNode* b = real_shape_nodes.second;
    MulNode* node = new MulNode(a, b, name, a->get_shape(), dtype);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a, b});
    return node;
}

SubNode* TFMath::subtract(Tensor *x, Tensor *y, std::string name){
    std::string dtype = dtype_check(x, y);
    auto real_shape_nodes = element_wise_shape_transfer(x, y);
    BaseNode* a = real_shape_nodes.first;
    BaseNode* b = real_shape_nodes.second;
    SubNode* node = new SubNode(a, b, name, a->get_shape(), dtype);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a, b});
    return node;
}