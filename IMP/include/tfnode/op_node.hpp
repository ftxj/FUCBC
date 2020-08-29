#pragma once

#include<string>
#include<vector>
#include<map>
#include<set>
#include <assert.h>
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

class OpNode : public BaseNode {
public:
    OpNode() : BaseNode() {}
    OpNode(std::string name, std::string type) : 
        BaseNode(name, type) {}
    OpNode(std::string name, std::string dtype, std::string type) : 
        BaseNode(name, dtype, type) {}
    OpNode(std::string name, Shape shape, std::string dtype, std::string type) : 
        BaseNode(name, shape, dtype, type) {}
};

class ElementWiseOp {

};

class UnaryOpNode : public OpNode {
private:
    BaseNode *x_;
public:
    UnaryOpNode(BaseNode *x) : OpNode(), x_(x) {
        reshape(x_);
        redtype(x_);
    }
    UnaryOpNode(BaseNode *x, std::string name, std::string type) : OpNode(name, type), x_(x) {
        reshape(x_);
        redtype(x_);
    }
    UnaryOpNode(BaseNode *x, std::string name, std::string dtype, std::string type) : OpNode(name, dtype, type), x_(x) {
        reshape(x_);
    }
    UnaryOpNode(BaseNode *x, std::string name, Shape shape, std::string dtype, std::string type) : 
        OpNode(name, shape, dtype, type), x_(x) {}
};

class BinaryOpNode : public OpNode {
private:
    BaseNode *x_;
    BaseNode *y_;
public:
    BinaryOpNode(BaseNode *x, BaseNode *y) : OpNode(), x_(x), y_(y) {
        // TODO
        reshape(x_);
        redtype(x_);
    }
    BinaryOpNode(BaseNode *x, BaseNode *y, std::string name, std::string type) 
        : OpNode(name, type), x_(x), y_(y) {
        reshape(x_);
        redtype(x_);
    }
    BinaryOpNode(BaseNode *x, BaseNode *y, std::string name, std::string dtype, std::string type) 
        : OpNode(name, dtype, type), x_(x),  y_(y) {
        reshape(x_);
    }
    BinaryOpNode(BaseNode *x, BaseNode *y, std::string name, Shape shape, std::string dtype, std::string type) : 
        OpNode(name, shape, dtype, type), x_(x), y_(y) {}
};

// AddNode* operator+(Tensor &x,  Tensor &y);

class TernaryOpNode : public OpNode {
private:
    BaseNode *x_;
    BaseNode *y_;
    BaseNode *z_;
public:
    TernaryOpNode(BaseNode *x, BaseNode *y, BaseNode *z) : OpNode(), x_(x), y_(y), z_(z) {}
    TernaryOpNode(BaseNode *x, BaseNode *y, BaseNode *z, std::string name, Shape shape, std::string dtype, std::string type) : 
        OpNode(name, shape, dtype, type), x_(x), y_(y), z_(z) {}
};

// class NNOpNode : public OpNode {

// };

class AbsNode : public UnaryOpNode {
public:
    AbsNode(BaseNode *x, std::string name, Shape shape, std::string dtype) : 
        UnaryOpNode(x, name, shape, dtype,"AbsNode") {}
};

class ExpNode : public UnaryOpNode {
public:
    ExpNode(BaseNode *x, std::string name, Shape shape, std::string dtype) : 
        UnaryOpNode(x, name, shape, dtype,"ExpNode") {}
};

class SigmoidNode : public UnaryOpNode {
public:
    SigmoidNode(BaseNode *x, std::string name, Shape shape, std::string dtype) : 
        UnaryOpNode(x, name, shape, dtype,"SigmodNode") {}
};

class SqrtNode : public UnaryOpNode {
public:
    SqrtNode(BaseNode *x, std::string name, Shape shape, std::string dtype) : 
        UnaryOpNode(x, name, shape, dtype,"SqrtNode") {}
};

class SquareNode : public UnaryOpNode {
public:
    SquareNode(BaseNode *x, std::string name, Shape shape, std::string dtype) : 
        UnaryOpNode(x, name, shape, dtype,"SquareNode") {}
};

class ReShapeNode : public UnaryOpNode {
public:
    ReShapeNode(BaseNode *x, std::string name, Shape shape, std::string dtype) : 
        UnaryOpNode(x, name, shape, dtype, "ReShapeNode") {}
};

// add node for MatMul
class TransposeNode : public UnaryOpNode {
public:
    TransposeNode(BaseNode *x, std::string name, Shape shape, std::string dtype) : 
        UnaryOpNode(x, name, shape, dtype,"TransposeNode") {}
};

// TODO reduce shape and expand shape class
class ArgMinNode : public UnaryOpNode {
private:
    int axis_;
public:
    ArgMinNode(BaseNode *x, int axis, std::string name, Shape shape, std::string dtype) : 
        UnaryOpNode(x, name, shape, dtype,"ArgMinNode") {}
};

class ExpandDimsNode : public UnaryOpNode {
private:
    int axis_;
public:
    ExpandDimsNode(BaseNode *x, int axis, std::string name, Shape shape, std::string dtype) : 
        UnaryOpNode(x, name, shape, dtype, "ExpandDims"), axis_(axis) {}
};

class AddNode : public BinaryOpNode, ElementWiseOp {
public:
    AddNode(BaseNode *x, BaseNode *y, std::string name, Shape shape, std::string dtype) : 
        BinaryOpNode(x, y, name, shape, dtype, "Add") {}
};

class DivNode : public BinaryOpNode, ElementWiseOp {
public:
    DivNode(BaseNode *x, BaseNode *y, std::string name, Shape shape, std::string dtype) : 
        BinaryOpNode(x, y, name, shape, dtype, "DivNode") {}
};

class FloorDivNode : public BinaryOpNode, ElementWiseOp {
public:
    FloorDivNode(BaseNode *x, BaseNode *y, std::string name, Shape shape, std::string dtype) : 
        BinaryOpNode(x, y, name, shape, dtype, "FloorDivNode") {}
};

class LessNode : public BinaryOpNode, ElementWiseOp{
public:
    LessNode(BaseNode *x, BaseNode *y, std::string name, Shape shape, std::string dtype) : 
        BinaryOpNode(x, y, name, shape, dtype, "LessNode") {}
};

class MulNode : public BinaryOpNode, ElementWiseOp {
public:
    MulNode(BaseNode *x, BaseNode *y, std::string name, Shape shape, std::string dtype) : 
        BinaryOpNode(x, y, name, shape, dtype, "MulNode") {}
};

class SubNode : public BinaryOpNode, ElementWiseOp {
public:
    SubNode(BaseNode *x, BaseNode *y, std::string name, Shape shape, std::string dtype) : 
        BinaryOpNode(x, y, name, shape, dtype, "SubNode") {}
};

class MatMulNode : public BinaryOpNode {
public:
    MatMulNode(BaseNode *x, BaseNode *y, std::string name, Shape shape, std::string dtype) : 
        BinaryOpNode(x, y, name, shape, dtype, "MatMulNode") {}
};

class TensordotNode : public BinaryOpNode {
public:
    TensordotNode(BaseNode *x, BaseNode *y, std::string name, Shape shape, std::string dtype) : 
        BinaryOpNode(x, y, name, shape, dtype, "TensordotNode") {}
};

// class Conv2DNode : public NNOpNode {
    
// };