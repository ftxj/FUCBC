#include "base_node.h"
#include <map>
#include <string>
using namespace std;

class Print : public Pass {
private:
    map<BaseNode*, bool> node_meeting;
    
    map<string, string> color_config;
    map<string, string> fontcolor_config;
    map<string, string> shape_config;
    map<string, string> label_config;
    map<string, string> fillcolor_config;

    string number;
    bool first_meeting(BaseNode*);
public:
    void exec(BaseNode*, int);

    void exec(ConstNode* u, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(PlaceholderNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(VariableNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }

    void exec(AbsNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(AddNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(ArgMinNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(DivNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(ExpNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(FloorDivNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(LessNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(MulNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(RealDivNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(SigmoidNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(SqrtNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(SquareNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(SubNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(SumNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(Conv2DNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(ExpandDimNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(MatMulNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(ReshapeNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(TensordotNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }

    void exec(AssignNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(AssignAddNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(GatherNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(IdentityNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(PackNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(SelectNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(StackNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }
    void exec(NoOpNodeu, int l) { exec(static_cast<BaseNode*>(u), l); }

};