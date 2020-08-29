#pragma once

#include<string>
#include<vector>
#include<map>
#include<set>

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

template<typename ValueType>
class OpNode : public BaseNode<ValueType> {
public:
    OpNode() : BaseNode() {}
    OpNode(std::string name, Shape shape, std::string dtype, std::string type) : 
        BaseNode<ValueType>(name, shape, dtype, type) {}
};


// class UnaryOpNode : public OpNode {

// };

template<typename ValueType>
class BinaryOpNode : public OpNode<ValueType> {
private:
    BaseNode &x_;
    BaseNode &y_;
public:
    BinaryOpNode(BaseNode &x, BaseNode &y) : OpNode(), x_(x), y_(y) {}
    BinaryOpNode(BaseNode &x, BaseNode &y, std::string name, Shape shape, std::string dtype, std::string type) : 
        OpNode<ValueType>(name, shape, dtype, type), x_(x), y_(y) {}
};

template<typename ValueType>
class AddNode : public BinaryOpNode<ValueType> {
public:
    AddNode(BaseNode &x, BaseNode &y, std::string name, Shape shape, std::string dtype) : 
        BinaryOpNode<ValueType>(x, y name, shape, dtype, "Add") {}
};

template<typename V1, typename V2>
AddNode<V1>& operator+(BaseNode<V1> &x, BaseNode<V2> &y) {
    assert(x.dtype_check(y));
    assert(x.shape_equal(y));
    AddNode<V1>* node = new AddNode<V1>(x, y, "", x.get_shape(), x.get_dtype());
    node->add_predecessors(&x);
    node->add_predecessors(&y);
    x.add_predecessors(node);
    y.add_predecessors(node);
    return *node;
}

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