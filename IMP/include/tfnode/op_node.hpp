#pragma once

#include<string>
#include<vector>
#include<map>
#include<set>
#include <functional>
#include "error/assert.hpp"
#include "tfnode/base_node.hpp"

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
int generate_counter();
class BasicOp {
private:
    int op_code_;
public:
    BasicOp(int op_code) : op_code_(op_code) {}
    static const std::map<std::string, int> op_code_map;
};

template<typename T>
class UnaryOp : public BasicOp {
private:
    Tensor<T> *x_;
public:
    UnaryOp(Tensor<T> *x, int op_code) : x_(x), BasicOp(op_code) {}
};

template<typename T1, typename T2>
class BinaryOp : public BasicOp {
private:
    Tensor<T1> *x_;
    Tensor<T2> *y_;
    int op_code_;
public:
    BinaryOp(Tensor<T1> *x, Tensor<T2> *y, int op_code) : BasicOp(op_code), x_(x), y_(y) {}
};

template<typename T1, typename T2, typename T3>
class TernaryOp : public BasicOp {
private:
    Tensor<T1> *x_;
    Tensor<T2> *y_;
    Tensor<T3> *z_;
public:
    TernaryOp(Tensor<T1> *x, Tensor<T2> *y, Tensor<T3> *z, int op_code) : BasicOp(op_code), x_(x), y_(y), z_(z) {}
};

template<typename T1, typename T2>
class ElementWiseOpNode : public BinaryOp<T1, T1>, public TensorNode<T2> {
public:
    ElementWiseOpNode(TensorNode<T1> *x, TensorNode<T1> *y, int op_code, 
        const std::string &name, const std::string &type) : 
        BinaryOp<T1, T1>(x, y, op_code), 
        TensorNode<T2>(name, element_wise_shape(x->get_shape(), y->get_shape()), type) {
        }
};

template<typename T>
class UnReduceUnaryOpNode : public UnaryOp<T>, public TensorNode<T> {
public:
    UnReduceUnaryOpNode(TensorNode<T> *x, int op_code, const std::string &name, const std::string &type) : 
        UnaryOp<T>(x, op_code), 
        TensorNode<T>(name, x->get_shape(), type) {
    }
};

template<typename T>
class ReduceUnaryOpNode : public UnaryOp<T>, public TensorNode<T> {
private:
    int axis_;
public:
    ReduceUnaryOpNode(TensorNode<T> *x, int op_code, int axis, const std::string &name, const std::string &type) : 
        UnaryOp<T>(x, op_code), 
        TensorNode<T>(name, x->get_shape().shrink_on(axis), type), axis_(axis) {
    }
};

template<typename T>
class TransShapeUnaryOpNode : public UnaryOp<T>, public TensorNode<T> {
public:
    TransShapeUnaryOpNode(TensorNode<T> *x, int op_code, Shape shape, const std::string &name, const std::string &type) : 
        UnaryOp<T>(x, op_code), 
        TensorNode<T>(name, shape, type) {
    }
};

// class NNOpNode : public BasicOp {

// };

template<typename T>
class AbsNode : public UnReduceUnaryOpNode<T> {
public:
    AbsNode(TensorNode<T> *x, const std::string &name = "Abs_"+std::to_string(generate_counter()), const std::string &type = "AbsNode") : 
        UnReduceUnaryOpNode<T>(x, BasicOp::op_code_map.find("Abs")->second, name, type) {
            generate_counter();
        }
    
};

template<typename T>
class ExpNode : public UnReduceUnaryOpNode<T> {
public:
    ExpNode(TensorNode<T> *x, const std::string &name = "Exp_"+std::to_string(generate_counter()), const std::string &type = "ExpNode") : 
        UnReduceUnaryOpNode<T>(x, BasicOp::op_code_map.find("Exp")->second, name, type) {
            generate_counter();
        }
    
};

template<typename T>
class SigmoidNode : public UnReduceUnaryOpNode<T> {
public:
    SigmoidNode(TensorNode<T> *x, const std::string &name = "Sigmod_"+std::to_string(generate_counter()), const std::string &type = "SigmoidNode") : 
        UnReduceUnaryOpNode<T>(x, BasicOp::op_code_map.find("Sigmod")->second, name, type) {
            generate_counter();
        }
    
};


template<typename T>
class SqrtNode : public UnReduceUnaryOpNode<T> {
public:
    SqrtNode(TensorNode<T> *x, const std::string &name = "Sqrt_"+std::to_string(generate_counter()), const std::string &type = "SqrtNode") : 
        UnReduceUnaryOpNode<T>(x, BasicOp::op_code_map.find(std::string("Sqrt"))->second, name, type) {
            generate_counter();
        }
    
};

template<typename T>
class SquareNode : public UnReduceUnaryOpNode<T> {
public:
    SquareNode(TensorNode<T> *x, const std::string &name = "Square_"+std::to_string(generate_counter()), const std::string &type = "SquareNode") : 
        UnReduceUnaryOpNode<T>(x, BasicOp::op_code_map.find("Square")->second, name, type) {
            generate_counter();
        }
    
};

template<typename T>
class ReShapeNode : public TransShapeUnaryOpNode<T> {
public:
    ReShapeNode(TensorNode<T> *x, Shape shape, const std::string &name = "ReShape_"+std::to_string(generate_counter()), const std::string &type = "ReShapeNode") : 
        TransShapeUnaryOpNode<T>(x, BasicOp::op_code_map.find("ReShape")->second, shape, name, type) {
            generate_counter();
        }
    
};

template<typename T>
class TransposeNode : public TransShapeUnaryOpNode<T> {
public:
    TransposeNode(TensorNode<T> *x, Shape shape, const std::string &name = "Transpose_"+std::to_string(generate_counter()), const std::string &type = "TransposeNode") : 
        TransShapeUnaryOpNode<T>(x, BasicOp::op_code_map.find("Transpose")->second, shape, name, type) {
            generate_counter();
        }
    
};

template<typename T>
class ArgMinNode : public ReduceUnaryOpNode<T> {
public:
    ArgMinNode(TensorNode<T> *x, int axis, const std::string &name) : 
        ReduceUnaryOpNode<T>(x, BasicOp::op_code_map.find("ArgMin")->second, axis, name,"ArgMinNode") {}
};

template<typename T>
class ExpandDimsNode : public TransShapeUnaryOpNode<T> {
private:
    int axis_;
public:
    ExpandDimsNode(TensorNode<T> *x, int axis, const std::string &name) : 
        TransShapeUnaryOpNode<T>(x, BasicOp::op_code_map.find("ExpandDims")->second, x->get_shape().expand_on(axis), "ExpandDims"), axis_(axis) {}
};

template<typename T>
class AddNode : public ElementWiseOpNode<T, T> {
public:
    AddNode(TensorNode<T> *x, TensorNode<T> *y, const std::string &name) : 
        ElementWiseOpNode<T, T>(x, y, BasicOp::op_code_map.find("Add")->second, name, "AddNode") {}
};

template<typename T>
class SubNode : public ElementWiseOpNode<T, T> {
public:
    SubNode(TensorNode<T> *x, TensorNode<T> *y, const std::string &name) : 
        ElementWiseOpNode<T, T>(x, y, BasicOp::op_code_map.find("Sub")->second, name, "SubNode") {}
};

template<typename T>
class MulNode : public ElementWiseOpNode<T, T> {
public:
    MulNode(TensorNode<T> *x, TensorNode<T> *y, const std::string &name) : 
        ElementWiseOpNode<T, T>(x, y, BasicOp::op_code_map.find("Mul")->second, name, "MulNode") {}
};

template<typename T>
class DivNode : public ElementWiseOpNode<T, T> {
public:
    DivNode(TensorNode<T> *x, TensorNode<T> *y, const std::string &name) : 
        ElementWiseOpNode<T, T>(x, y, BasicOp::op_code_map.find("Div")->second, name, "DivNode") {}
};

template<typename T>
class FloorDivNode : public ElementWiseOpNode<T, T> {
public:
    FloorDivNode(TensorNode<T> *x, TensorNode<T> *y, const std::string &name) : 
        ElementWiseOpNode<T, T>(x, y, BasicOp::op_code_map.find("FloorDiv")->second, name, "FloorDivNode") {}
};

template<typename T>
class LessNode : public ElementWiseOpNode<T, bool> {
public:
    LessNode(TensorNode<T> *x, TensorNode<T> *y, const std::string &name) : 
        ElementWiseOpNode<T, bool>(x, y, BasicOp::op_code_map.find("Less")->second, name, "LessNode") {}
};


template<typename T>
class MatMulNode : public BinaryOp<T, T>, public TensorNode<T> {
public:
    MatMulNode(TensorNode<T> *x, TensorNode<T> *y, const std::string &name, Shape shape) : 
        BinaryOp<T, T>(x, y, BasicOp::op_code_map.find("MatMul")->second), TensorNode<T>(name, shape, "MatMulNode") {}
};


template<typename T>
class TensordotNode : public BinaryOp<T, T>, public TensorNode<T> {
public:
    // TODO fix bug here
    TensordotNode(TensorNode<T> *x, TensorNode<T> *y, const std::string &name) : 
        BinaryOp<T, T>(x, y, BasicOp::op_code_map.find("MatMul")->second), 
        TensorNode<T>(name, Shape::get_matmul_shape(x->get_shape(), y->get_shape()), "TensordotNode") {}
};

// class Conv2DNode : public NNOpNode {
    
// };

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


typedef AbsNode<int> IntAbsNode;
typedef AddNode<int> IntAddNode;
typedef ArgMinNode<int> IntArgMinNode;
typedef DivNode<int> IntDivNode;
typedef ExpNode<int> IntExpNode;
typedef FloorDivNode<int> IntFloorDivNode;
typedef LessNode<int> IntLessNode;
typedef MulNode<int> IntMulNode;
//typedef RealDivNode<int> IntRealDivNode;
typedef SigmoidNode<int> IntSigmoidNode;
typedef SqrtNode<int> IntSqrtNode;
typedef SquareNode<int> IntSquareNode;
typedef SubNode<int> IntSubNode;
//typedef SumNode<int> IntSumNode;
//typedef Conv2DNode<int> IntConv2DNode;
typedef ExpandDimsNode<int> IntExpandDimsNode;
typedef MatMulNode<int> IntMatMulNode;
typedef ReShapeNode<int> IntReShapeNode;
typedef TensordotNode<int> IntTensordotNode;
