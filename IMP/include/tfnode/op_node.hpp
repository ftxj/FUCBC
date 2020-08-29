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
    OpNode(std::string name, Shape shape, std::string dtype, std::string type) : 
        BaseNode(name, shape, dtype, type) {}
};


// class UnaryOpNode : public OpNode {

// };

class BinaryOpNode : public OpNode {
private:
    BaseNode *x_;
    BaseNode *y_;
public:
    BinaryOpNode(BaseNode *x, BaseNode *y) : OpNode(), x_(x), y_(y) {}
    BinaryOpNode(BaseNode *x, BaseNode *y, std::string name, Shape shape, std::string dtype, std::string type) : 
        OpNode(name, shape, dtype, type), x_(x), y_(y) {}
};

class AddNode : public BinaryOpNode {
public:
    AddNode(BaseNode *x, BaseNode *y, std::string name, Shape shape, std::string dtype) : 
        BinaryOpNode(x, y, name, shape, dtype, "Add") {}
};

// AddNode* operator+(Tensor &x,  Tensor &y);

// class TernaryOpNode : public OpNode {

// };

// class NNOpNode : public OpNode {

// };


// class AbsNode : public UnaryOpNode {

// };


// class ArgMinNode : public UnaryOpNode {

// };

// class ExpNode : public UnaryOpNode {

// };

// class SigmoidNode : public UnaryOpNode {

// };

// class SqrtNode : public UnaryOpNode {

// };

// class SquareNode : public UnaryOpNode {

// };

// class ExpandDimsNode : public UnaryOpNode {

// };

// class MatMulNode : public BinaryOpNode {

// };

// class TensordotNode : public BinaryOpNode {

// };

// class ReShapeNode : public BinaryOpNode {

// };



// class DivNode : public BinaryOpNode {

// };

// class FloorDivNode : public BinaryOpNode {
//     // floor(x / y)
// }

// class LessNode : public BinaryOpNode {

// }

// class MulNode : public BinaryOpNode {
//     // floor(x / y)
// }

// class SubNode : public BinaryOpNode {
//     // floor(x / y)
// }

// class Conv2DNode : public NNOpNode {
    
// };