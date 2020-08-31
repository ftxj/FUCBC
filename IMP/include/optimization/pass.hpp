#pragma once
#include "tfnode/input_node.hpp"
#include "tfnode/control_node.hpp"
#include "tfnode/base_node.hpp"
#include "tfnode/op_node.hpp"


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

class Pass {

public:
    #define PURE_VIRTUAL_EXEC_INT(type) virtual void exec(Int##type##Node*, int) = 0; 
    virtual void handle_header() = 0;

    virtual void exec(Node*, int) = 0;
    PURE_VIRTUAL_EXEC_INT(Abs);
    PURE_VIRTUAL_EXEC_INT(Add);
    PURE_VIRTUAL_EXEC_INT(ArgMin);
    PURE_VIRTUAL_EXEC_INT(Div);
    PURE_VIRTUAL_EXEC_INT(Exp);
    PURE_VIRTUAL_EXEC_INT(FloorDiv);
    PURE_VIRTUAL_EXEC_INT(Less);
    PURE_VIRTUAL_EXEC_INT(Mul);
    //PURE_VIRTUAL_EXEC_INT(RealDiv);
    PURE_VIRTUAL_EXEC_INT(Sigmoid);
    PURE_VIRTUAL_EXEC_INT(Sqrt);
    PURE_VIRTUAL_EXEC_INT(Square);
    PURE_VIRTUAL_EXEC_INT(Sub);
    //PURE_VIRTUAL_EXEC_INT(Sum);
    //PURE_VIRTUAL_EXEC_INT(Conv2D);
    PURE_VIRTUAL_EXEC_INT(ExpandDims);
    PURE_VIRTUAL_EXEC_INT(MatMul);
    PURE_VIRTUAL_EXEC_INT(ReShape);
    PURE_VIRTUAL_EXEC_INT(Tensordot);
    virtual void handle_tail() = 0;
};