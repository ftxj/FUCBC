#pragma once
#include "tfnode/input_node.hpp"
#include "tfnode/control_node.hpp"
#include "tfnode/base_node.hpp"
#include "tfnode/op_node.hpp"

class Pass {
public:
    virtual void handle_header() = 0;
    virtual void exec(BaseNode*, int) = 0;
    virtual void exec(ConstantNode*, int) = 0;
    virtual void exec(PlaceholderNode*, int) = 0;
    virtual void exec(VariableNode*, int) = 0;
    
    virtual void exec(AddNode*, int) = 0;
    virtual void exec(TransposeNode*, int) = 0;
    virtual void exec(AbsNode*, int) = 0;
    virtual void exec(ArgMinNode*, int) = 0;
    virtual void exec(DivNode*, int) = 0;
    virtual void exec(ExpNode*, int) = 0;
    virtual void exec(FloorDivNode*, int) = 0;
    virtual void exec(LessNode*, int) = 0;
    virtual void exec(MulNode*, int) = 0;
    virtual void exec(SigmoidNode*, int) = 0;
    virtual void exec(SqrtNode*, int) = 0;
    virtual void exec(SquareNode*, int) = 0;
    virtual void exec(SubNode*, int) = 0;
    virtual void exec(ExpandDimsNode*, int) = 0;
    virtual void exec(MatMulNode*, int) = 0;
    virtual void exec(TensordotNode*, int) = 0;

    //virtual void exec(ReshapeNode*, int) = 0;
    //virtual void exec(Conv2DNode*, int) = 0;
    //virtual void exec(RealDivNode*, int) = 0;
    //virtual void exec(SumNode*, int) = 0;
    
    // void exec(AssignNode*, int) = 0;
    // void exec(AssignAddNode*, int) = 0;
    // void exec(GatherNode*, int) = 0;
    // void exec(IdentityNode*, int) = 0;
    // void exec(PackNode*, int) = 0;
    // void exec(SelectNode*, int) = 0;
    // void exec(StackNode*, int) = 0;
    // void exec(NoOpNode*, int) = 0;

    virtual void handle_tail() = 0;
};