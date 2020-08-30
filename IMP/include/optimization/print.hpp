#pragma once
#include "tfnode/base_node.hpp"
#include "tfnode/op_node.hpp"
#include "optimization/pass.hpp"
#include <map>
#include <string>
#include <set>
class Print : public Pass {
private:
    std::set<BaseNode*> node_meeting;
    
    std::map<std::string, std::string> color_config;
    std::map<std::string, std::string> fontcolor_config;
    std::map<std::string, std::string> shape_config;
    std::map<std::string, std::string> label_config;
    std::map<std::string, std::string> fillcolor_config;
    const std::string dot_pdf = "dot -ofile ";
    std::string number;
    bool first_meeting(BaseNode* n);
public:
    Print();
    virtual void handle_header();
    virtual void handle_tail();
    virtual void exec(BaseNode*, int);

    virtual void exec(ConstantNode* u, int l) { exec(static_cast<BaseNode*>(u), l); }
    virtual void exec(PlaceholderNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    virtual void exec(VariableNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }

    virtual void exec(TransposeNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    virtual void exec(AbsNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    virtual void exec(ArgMinNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    virtual void exec(ExpNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    virtual void exec(SqrtNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    virtual void exec(SquareNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    virtual void exec(SigmoidNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
  
    virtual void exec(AddNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    virtual void exec(SubNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    virtual void exec(MulNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    virtual void exec(DivNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    virtual void exec(FloorDivNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    virtual void exec(LessNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }

    virtual void exec(ExpandDimsNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    virtual void exec(MatMulNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    virtual void exec(TensordotNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    
    //virtual void exec(ReshapeNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    //virtual void exec(Conv2DNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    //virtual void exec(SumNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // virtual void exec(AssignNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // virtual void exec(AssignAddNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // virtual void exec(GatherNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // virtual void exec(IdentityNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // virtual void exec(PackNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // virtual void exec(SelectNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // virtual void exec(StackNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // virtual void exec(NoOpNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }

};