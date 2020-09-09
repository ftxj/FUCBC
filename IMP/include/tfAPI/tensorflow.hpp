#pragma once
#include "tfnode/op_node.hpp"
#include "tfnode/input_node.hpp"
#include "tfnode/dfg.hpp"
#include "optimization/print.hpp"
#include "data_structure/shape.hpp"
#include <iostream>

// class Abs;
// class Add;
// class ArgMin;
// class Div;
// class Exp;
// class FloorDiv;
// class Less;
// class Mul;
// class RealDiv;
// class Sigmoid;
// class Sqrt;
// class Square;
// class Sub;
// class Sum;
// class Conv2D;
// class ExpandDims;
// class MatMul;
// class Reshape;
// class Tensordot;

class TFMath {
private:
    DFG* dfg_;
    // template<typename T1, typename T2>
    // std::string dtype_check(Tensor<T1> *x, Tensor<T2> *y);
    template<typename T1, typename T2>
    Shape shape_check(Tensor<T1> *x, Tensor<T2> *y);
    template<typename T1, typename T2>
    std::pair<TensorNode<T1>*, TensorNode<T2>*> element_wise_shape_transfer(Tensor<T1> *x, Tensor<T2> *y);
public:
    TFMath() : dfg_(nullptr) {}
    TFMath(DFG* dfg) : dfg_(dfg) {}
    DFG* get_dfg() {return dfg_;}
    void set_dfg(DFG* dfg) {dfg_ = dfg; }
    template<typename T> AbsNode<T>* abs(Tensor<T> *x, std::string name = "");
    
    

    template<typename T> ArgMinNode<T>* argmin(Tensor<T> *x, int axis, std::string output_type="int64", std::string name = "");
    template<typename T> ExpNode<T>* exp(Tensor<T> *x, std::string name = "");
    template<typename T> ExpandDimsNode<T>* expand_dims(Tensor<T> *x, int axis, std::string name = "");
    template<typename T> ReShapeNode<T>* reshape(Tensor<T> *x, Shape shape, std::string name = "");
    template<typename T> SigmoidNode<T>* sigmod(Tensor<T> *x, std::string name = "");
    template<typename T> SqrtNode<T>* sqrt(Tensor<T> *x, std::string name = "");
    template<typename T> SquareNode<T>* square(Tensor<T> *x, std::string name = "");
    
    
    template<typename T> AddNode<T>* add(Tensor<T> *x, Tensor<T> *y, std::string name = "");
    
    template<typename T> MatMulNode<T>* matmul(Tensor<T> *a, Tensor<T> *b, 
            bool transpose_a = false, bool transpose_b = false,
            bool a_is_sparse = false, bool b_is_sparse = false,
            std::string name = ""
        );
    template<typename T> TensordotNode<T>* tensordot(Tensor<T> *x, Tensor<T> *y, std::vector<int> axes,  std::string name = "");

    template<typename T> DivNode<T>* div(Tensor<T> *x, Tensor<T> *y, std::string name = "");
    template<typename T> FloorDivNode<T>* floordiv(Tensor<T> *x, Tensor<T> *y, std::string name = "");
    template<typename T> LessNode<T>* less(Tensor<T> *x, Tensor<T> *y, std::string name = "");
    template<typename T> MulNode<T>* multiply(Tensor<T> *x, Tensor<T> *y, std::string name = "");
    template<typename T> SubNode<T>* subtract(Tensor<T> *x, Tensor<T> *y, std::string name = "");


    template<typename T> AbsNode<T>* reduce_sum(Tensor<T> *x, int axis, std::string name = "");
    template<typename T> TransposeNode<T>* transpose(Tensor<T> *x, std::string name = "");
    template<typename T> TensorNode<T>* unsorted_segment_sum(Tensor<T> *x, Tensor<T> *seg, int num_segmemt, std::string name ="");
    template<typename T> TensorNode<T>* reduce_any(Tensor<T>* input_tensor, int axis, bool keepdims, std::string name = "");
    template<typename T> TensorNode<T>* not_equal(Tensor<T>* x, Tensor<T>* y, std::string name = "");
    template<typename T> TensorNode<T>* negative(Tensor<T>* x, int axis, std::string name = "");
    template<typename T> TensorNode<T>* top_k(Tensor<T>* x, int k, bool sorted, std::string name = "");

};

class TensorFlow {
private:
    Print* print;
    DFG* dfg_;
public:
    TensorFlow();
    TFMath math;
    template<typename T> ConstantNode<T>* constant(const NDArray<T> &value, Shape shape = Shape(), std::string name = "Const");    
    template<typename T> PlaceholderNode<T>* placeholder(Shape shape = Shape(), std::string name = "");    
    template<typename T> VariableNode<T>* Variable(const NDArray<T> &value, Shape shape = Shape(), std::string name = "");    
    template<typename T> ConstantNode<T>* ones(Shape shape = Shape(), std::string name = "");    
    template<typename T> ConstantNode<T>* zeros(Shape shape = Shape(), std::string name = "");    
    
    template<typename T> Tensor<T>* read_dataset(std::string filename, bool one_hot);    
    
    void run();
};

template<typename T> 
ConstantNode<T>* TensorFlow::constant(const NDArray<T> &value, Shape shape, std::string name) {
    ConstantNode<T>* node = new ConstantNode<T>(value, shape, name);
    node->set_dfg(dfg_);
    dfg_->add_input_node(node);
    return node;
}

template<typename T> 
PlaceholderNode<T>* TensorFlow::placeholder(Shape shape, std::string name) {
    PlaceholderNode<T>* node = new PlaceholderNode<T>(shape, name);
    node->set_dfg(dfg_);
    dfg_->add_input_node(node);
    return node;
} 

template<typename T> 
VariableNode<T>* TensorFlow::Variable(const NDArray<T> &value, Shape shape, std::string name) {
    VariableNode<T>* node = new VariableNode<T>(value, shape, name);
    node->set_dfg(dfg_);
    dfg_->add_input_node(node);
    return node;
}

void TensorFlow::run() {
    dfg_->topological_pass(print);
}

// template<typename T1, typename T2> 
// std::string TFMath::dtype_check(Tensor<T1> *x, Tensor<T2> *y) {
//     // TODO
//     return x->get_dtype();
// }

template<typename T1, typename T2> 
Shape TFMath::shape_check(Tensor<T1> *x, Tensor<T2> *y) {
    // TODO
    return x->get_shape();
}

template<typename T1, typename T2>
std::pair<TensorNode<T1>*, TensorNode<T2>*> TFMath::element_wise_shape_transfer(Tensor<T1> *x, Tensor<T2> *y) {
    TensorNode<T1>* a = static_cast<TensorNode<T1>*>(x);
    TensorNode<T2>* b = static_cast<TensorNode<T2>*>(y);
    if(a == nullptr || b == nullptr) {
        assert_msg(0, "static_cast<> fail: x=" << x << ",y=" << y);
    }
    if(a->get_shape() > b->get_shape()) {
        ReShapeNode<T2>* node = new ReShapeNode<T2>(b, a->get_shape(), "r1");
        dfg_->add_node(node, {b});
        return {a, node};
    }
    else if(a->get_shape() < b->get_shape()) {
        ReShapeNode<T1>* node = new ReShapeNode<T1>(a, b->get_shape(), "r2");
        dfg_->add_node(node, {a});
        return {node, b};
    }
    else {
        return {a, b};
    }
}

template<typename T> 
AbsNode<T>* TFMath::abs(Tensor<T>  *x, std::string name) {
    TensorNode<T> * a = static_cast<TensorNode<T>*>(x);
    AbsNode<T> * node = new AbsNode<T>(a, name);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a});
    return node;
}

template<typename T> 
ArgMinNode<T>* TFMath::argmin(Tensor<T> *x, int axis, std::string output_type, std::string name) {
    TensorNode<T>* a = static_cast<TensorNode<T>*>(x);
    ArgMinNode<T>* node = new ArgMinNode<T>(a, axis, name);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a});
    return node;
}

template<typename T> 
ExpNode<T>* TFMath::exp(Tensor<T> *x, std::string name) {
    TensorNode<T>* a = static_cast<TensorNode<T>*>(x);
    ExpNode<T>* node = new ExpNode<T>(a, name);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a});
    return node;
}

template<typename T> 
ExpandDimsNode<T>* TFMath::expand_dims(Tensor<T> *x, int axis, std::string name) {
    TensorNode<T>* a = static_cast<TensorNode<T>*>(x);
    ExpandDimsNode<T>* node = new ExpandDimsNode<T>(a, axis, name);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a});
    return node;
}

template<typename T> 
ReShapeNode<T>* TFMath::reshape(Tensor<T> *x, Shape shape, std::string name) {
    TensorNode<T>* a = static_cast<TensorNode<T>*>(x);
    ReShapeNode<T>* node = new ReShapeNode<T>(a, name, "rrr");
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a});
    return node;
}

template<typename T> 
SigmoidNode<T>* TFMath::sigmod(Tensor<T> *x, std::string name) {
    TensorNode<T>* a = static_cast<TensorNode<T>*>(x);
    SigmoidNode<T>* node = new SigmoidNode<T>(a, name);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a});
    return node;
}

template<typename T> 
SqrtNode<T>* TFMath::sqrt(Tensor<T> *x, std::string name) {
    TensorNode<T>* a = static_cast<TensorNode<T>*>(x);
    SqrtNode<T>* node = new SqrtNode<T>(a, name);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a});
    return node;
}

template<typename T> 
SquareNode<T>* TFMath::square(Tensor<T> *x, std::string name) {
    TensorNode<T>* a = static_cast<TensorNode<T>*>(x);
    SquareNode<T>* node = new SquareNode<T>(a, name);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a});
    return node;
}

template<typename T> 
AddNode<T>* TFMath::add(Tensor<T> *x, Tensor<T> *y, std::string name) {
    //std::string dtype = dtype_check(x, y);
    auto real_shape_nodes = element_wise_shape_transfer(x, y);
    TensorNode<T>* a = real_shape_nodes.first;
    TensorNode<T>* b = real_shape_nodes.second;
    AddNode<T>* node = new AddNode<T>(a, b, name);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a, b});
    return node;
}

template<typename T> 
MatMulNode<T>* TFMath::matmul(Tensor<T> *x, Tensor<T> *y, 
        bool transpose_a, bool transpose_b,
        bool a_is_sparse, bool b_is_sparse,
        std::string name
    ) {
    //std::string dtype = dtype_check(x, y);
    //TODO shape check
    TensorNode<T>* a = static_cast<TensorNode<T>*>(x);
    TensorNode<T>* b = static_cast<TensorNode<T>*>(y);
    if(transpose_a) {
        TransposeNode<T>* trans_a_node = new TransposeNode<T>(a, a->get_shape());
        dfg_->add_node(trans_a_node, {a});
        a = trans_a_node;
    }
    if(transpose_b) {
        TransposeNode<T>* trans_b_node = new TransposeNode<T>(b, b->get_shape()); 
        dfg_->add_node(trans_b_node, {b});
        b = trans_b_node;
    }
    Shape c_shape;
    MatMulNode<T>* node = new MatMulNode<T>(a, b, name, c_shape);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a, b});
    return node;
}

template<typename T> 
TensordotNode<T>* TFMath::tensordot(Tensor<T> *x, Tensor<T> *y, std::vector<int> axes,  std::string name) {
    // TODO rewrite this bug 
    //std::string dtype = dtype_check(x, y);
    auto real_shape_nodes = element_wise_shape_transfer(x, y);
    TensorNode<T>* a = real_shape_nodes.first;
    TensorNode<T>* b = real_shape_nodes.second;
    TensordotNode<T>* node = new TensordotNode<T>(a, b, name);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a, b});
    return node;
}

template<typename T> 
DivNode<T>* TFMath::div(Tensor<T> *x, Tensor<T> *y, std::string name) {
    //std::string dtype = dtype_check(x, y);
    auto real_shape_nodes = element_wise_shape_transfer(x, y);
    TensorNode<T>* a = real_shape_nodes.first;
    TensorNode<T>* b = real_shape_nodes.second;
    DivNode<T>* node = new DivNode<T>(a, b, name);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a, b});
    return node;
}

template<typename T> 
FloorDivNode<T>* TFMath::floordiv(Tensor<T> *x, Tensor<T> *y, std::string name) {
    //std::string dtype = dtype_check(x, y);
    auto real_shape_nodes = element_wise_shape_transfer(x, y);
    TensorNode<T>* a = real_shape_nodes.first;
    TensorNode<T>* b = real_shape_nodes.second;
    FloorDivNode<T>* node = new FloorDivNode<T>(a, b, name);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a, b});
    return node;
}

template<typename T> 
LessNode<T>* TFMath::less(Tensor<T> *x, Tensor<T> *y, std::string name) {
    //std::string dtype = dtype_check(x, y);
    auto real_shape_nodes = element_wise_shape_transfer(x, y);
    TensorNode<T>* a = real_shape_nodes.first;
    TensorNode<T>* b = real_shape_nodes.second;
    LessNode<T>* node = new LessNode<T>(a, b, name);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a, b});
    return node;
}

template<typename T> 
MulNode<T>* TFMath::multiply(Tensor<T> *x, Tensor<T> *y, std::string name) {
    //std::string dtype = dtype_check(x, y);
    auto real_shape_nodes = element_wise_shape_transfer(x, y);
    TensorNode<T>* a = real_shape_nodes.first;
    TensorNode<T>* b = real_shape_nodes.second;
    MulNode<T>* node = new MulNode<T>(a, b, name);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a, b});
    return node;
}

template<typename T> 
SubNode<T>* TFMath::subtract(Tensor<T> *x, Tensor<T> *y, std::string name){
    //std::string dtype = dtype_check(x, y);
    auto real_shape_nodes = element_wise_shape_transfer(x, y);
    TensorNode<T>* a = real_shape_nodes.first;
    TensorNode<T>* b = real_shape_nodes.second;
    SubNode<T>* node = new SubNode<T>(a, b, name);
    node->set_dfg(dfg_);
    dfg_->add_node(node, {a, b});
    return node;
}
