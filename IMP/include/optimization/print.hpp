#pragma once
#include "tfnode/base_node.hpp"
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

    std::string number;
    bool first_meeting(BaseNode* n) { return node_meeting.find(n) == node_meeting.end()?false : true; }
public:
    Print() {}
    virtual void exec(BaseNode*, int);

    virtual void exec(ConstantNode* u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(PlaceholderNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(VariableNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }

    //void exec(AbsNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    virtual void exec(AddNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(ArgMinNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(DivNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(ExpNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(FloorDivNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(LessNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(MulNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(RealDivNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(SigmoidNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(SqrtNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(SquareNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(SubNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(SumNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(Conv2DNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(ExpandDimNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(MatMulNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(ReshapeNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(TensordotNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }

    // void exec(AssignNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(AssignAddNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(GatherNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(IdentityNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(PackNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(SelectNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(StackNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    // void exec(NoOpNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }

};