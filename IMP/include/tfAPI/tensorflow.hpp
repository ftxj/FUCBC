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
    std::string dtype_check(Tensor *x, Tensor *y);
    Shape shape_check(Tensor *x, Tensor *y);
    std::pair<BaseNode*, BaseNode*> element_wise_shape_transfer(Tensor *x, Tensor *y);
public:
    TFMath() : dfg_(nullptr) {}
    TFMath(DFG* dfg) : dfg_(dfg) {}
    DFG* get_dfg() {return dfg_;}
    void set_dfg(DFG* dfg) {dfg_ = dfg; }
    AbsNode* abs(Tensor *x, std::string name = "");
    ArgMinNode* argmin(Tensor *x, int axis, std::string output_type="int64", std::string name = "");
    ExpNode* exp(Tensor *x, std::string name = "");
    ExpandDimsNode* expand_dims(Tensor *x, int axis, std::string name = "");
    ReShapeNode* reshape(Tensor *x, Shape shape, std::string name = "");
    SigmoidNode* sigmod(Tensor *x, std::string name = "");
    SqrtNode* sqrt(Tensor *x, std::string name = "");
    SquareNode* square(Tensor *x, std::string name = "");

    AddNode* add(Tensor *x, Tensor *y, std::string name = "");
    
    MatMulNode* matmul(Tensor *a, Tensor *b, 
            bool transpose_a = false, bool transpose_b = false,
            bool a_is_sparse = false, bool b_is_sparse = false,
            std::string name = ""
        );
    TensordotNode* tensordot(Tensor *x, Tensor *y, std::vector<int> axes,  std::string name = "");

    DivNode* div(Tensor *x, Tensor *y, std::string name = "");
    FloorDivNode* floordiv(Tensor *x, Tensor *y, std::string name = "");
    LessNode* less(Tensor *x, Tensor *y, std::string name = "");
    MulNode* multiply(Tensor *x, Tensor *y, std::string name = "");
    SubNode* subtract(Tensor *x, Tensor *y, std::string name = "");
};

class TensorFlow {
private:
    Print* print;
    DFG* dfg_;
public:
    TensorFlow();
    TFMath math;
    ConstantNode* constant(NDArray value, std::string dtype = "unknow", Shape shape = Shape(), std::string name = "Const");    
    PlaceholderNode* placeholder(std::string dtype = "unknow", Shape shape = Shape(), std::string name = "");    
    VariableNode* Variable(NDArray value, std::string dtype = "unknow", Shape shape = Shape(), std::string name = "");    
    void run();
};