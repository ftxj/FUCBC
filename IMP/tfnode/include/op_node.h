#include<string>
#include<vector>
#include<map>
#include<set>

#include "base_node.hpp"

using namespace std;

class Abs;
class Add;
class ArgMin;
class Div;
class Exp;
class FloorDiv;
class Less;
class Mul;
class RealDiv;
class Sigmoid;
class Sqrt;
class Square;
class Sub;
class Sum;
class Conv2D;
class ExpandDims;
class MatMul;
class Reshape;
class Tensordot;

class OpNode : public BaseNode {

};


class UnaryOpNode : public OpNode {

};

class BinaryOpNode : public OpNode {
private:
    BaseNode &x_;
    BaseNode &y_;
public:
    BinaryOpNode(BaseNode &x, BaseNode &y) : x_(x), y_(y) {}
};

class TernaryOpNode : public OpNode {

};

class NNOpNode : public OpNode {

};


class AbsNode : public UnaryOpNode {

};


class ArgMinNode : public UnaryOpNode {

};

class ExpNode : public UnaryOpNode {

};

class SigmoidNode : public UnaryOpNode {

};

class SqrtNode : public UnaryOpNode {

};

class SquareNode : public UnaryOpNode {

};

class ExpandDimsNode : public UnaryOpNode {

};

class MatMulNode : public BinaryOpNode {

};

class TensordotNode : public BinaryOpNode {

};

class ReShapeNode : public BinaryOpNode {

};

class AddNode : public BinaryOpNode {
public:
    AddNode(BaseNode &x, BaseNode &y, std::string name = "") : BinaryOpNode(x, y) {} 
};

AddNode operator+(BaseNode &x, BaseNode &y) {
    return AddNode(x, y);
}


class DivNode : public BinaryOpNode {

};

class FloorDivNode : public BinaryOpNode {
    // floor(x / y)
}

class LessNode : public BinaryOpNode {

}

class MulNode : public BinaryOpNode {
    // floor(x / y)
}

class SubNode : public BinaryOpNode {
    // floor(x / y)
}

class Conv2DNode : public NNOpNode {
    
};