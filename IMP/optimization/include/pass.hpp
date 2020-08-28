#include "base_node.h"
#include "op_node.h"
#include "input_node.h"
#include "control_node.h"


class Pass {
public:
    void exec(BaseNode*, int);

    void exec(ConstNode*, int);
    void exec(PlaceholderNode*, int);
    void exec(VariableNode*, int);

    void exec(AbsNode*, int);
    void exec(AddNode*, int);
    void exec(ArgMinNode*, int);
    void exec(DivNode*, int);
    void exec(ExpNode*, int);
    void exec(FloorDivNode*, int);
    void exec(LessNode*, int);
    void exec(MulNode*, int);
    void exec(RealDivNode*, int);
    void exec(SigmoidNode*, int);
    void exec(SqrtNode*, int);
    void exec(SquareNode*, int);
    void exec(SubNode*, int);
    void exec(SumNode*, int);
    void exec(Conv2DNode*, int);
    void exec(ExpandDimNode*, int);
    void exec(MatMulNode*, int);
    void exec(ReshapeNode*, int);
    void exec(TensordotNode*, int);

    void exec(AssignNode*, int);
    void exec(AssignAddNode*, int);
    void exec(GatherNode*, int);
    void exec(IdentityNode*, int);
    void exec(PackNode*, int);
    void exec(SelectNode*, int);
    void exec(StackNode*, int);
    void exec(NoOpNode*, int);
};