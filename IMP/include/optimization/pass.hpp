#pragma once
#include "tfnode/input_node.hpp"
#include "tfnode/control_node.hpp"
#include "tfnode/base_node.hpp"
#include "tfnode/op_node.hpp"

class Pass {
public:
    virtual void exec(BaseNode*, int) = 0;
    virtual void exec(ConstantNode*, int) = 0;
    virtual void exec(AddNode*, int) = 0;

    // void exec(PlaceholderNode*, int);
    // void exec(VariableNode*, int);

    // void exec(AbsNode*, int);
    // void exec(ArgMinNode*, int);
    // void exec(DivNode*, int);
    // void exec(ExpNode*, int);
    // void exec(FloorDivNode*, int);
    // void exec(LessNode*, int);
    // void exec(MulNode*, int);
    // void exec(RealDivNode*, int);
    // void exec(SigmoidNode*, int);
    // void exec(SqrtNode*, int);
    // void exec(SquareNode*, int);
    // void exec(SubNode*, int);
    // void exec(SumNode*, int);
    // void exec(Conv2DNode*, int);
    // void exec(ExpandDimNode*, int);
    // void exec(MatMulNode*, int);
    // void exec(ReshapeNode*, int);
    // void exec(TensordotNode*, int);

    // void exec(AssignNode*, int);
    // void exec(AssignAddNode*, int);
    // void exec(GatherNode*, int);
    // void exec(IdentityNode*, int);
    // void exec(PackNode*, int);
    // void exec(SelectNode*, int);
    // void exec(StackNode*, int);
    // void exec(NoOpNode*, int);
};