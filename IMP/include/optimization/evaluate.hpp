#pragma once
#include "tfnode/pass.hpp"
#include "tfnode/op_node.hpp"
#include "tfnode/input_node.hpp"
#include "tfnode/base_node.hpp"

#include<string>
#include<vector>
#include<map>
#include<set>
#include <cmath>


class Evaluate : public Pass {
private:
    template<class T1, class T2, class T3>
    void exec_element_wise_base(BaseNode* node, std::function<T3 (T1, T2)> op) {
        std::vector<BaseNode*> pred = node->get_predecessors();
        assert_msg(pred.size() >= 2, "add node must have at least two pre");
        BaseNode* x = pred[0];
        NDArray res = x->get_values();
        for(int i = 1; i < pred.size(); ++i) {
            res.element_wist_op(pred[i]->get_values(), op);
        }
        node->set_values(res);
    }

    template<class T1, class T2>
    void exec_unary_op_base(BaseNode* node, std::function<T2 (T1)> op) {
        std::vector<BaseNode*> pred = node->get_predecessors();
        assert_msg(pred.size() == 1, "unary op");
        BaseNode* x = pred[0];
        NDArray res = x->get_values();
        res.unary_op(op);
        node->set_values(res);
    }

public:
    void exec(ConstNode* node, int l) {

    }

    virtual void exec(AbsNode* node, int l) {
        exec_unary_op_base(node, [](int a)->int {return a > 0? a : -a;});
    }

    virtual void exec(ExpNode *node, int l) { 
        exec_unary_op_base(node, [](int a)->float {return std::exp(a);});
    }

    virtual void exec(SqrtNode *node, int l) { 
        exec_unary_op_base(node, [](int a)->float {return std::sqrt(a);});
    }

    virtual void exec(SquareNode *node, int l) { 
        exec_unary_op_base(node, [](int a)->float {return std::pow(a, 2);});
    }

    virtual void exec(SigmoidNode *node, int l) { 
        exec_unary_op_base(node, [](int a)->float {return a/ (1+(a>0?a:-a));});
    }


    virtual void exec(TransposeNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
    virtual void exec(ArgMinNode *u, int l) { exec(static_cast<BaseNode*>(u), l); }
  

    virtual void exec(AddNode* node, int l) {
        exec_element_wise_base(node, [](int a, int b)->int {return a+b;});
    }

    virtual void exec(SubNode* node, int l) {
        exec_element_wise_base(node, [](int a, int b)->int {return a-b;});
    }
    
    virtual void exec(MulNode* node, int l) {
        exec_element_wise_base(node, [](int a, int b)->int {return a*b;});
    }
    
    virtual void exec(DivNode* node, int l) {
        exec_element_wise_base(node, [](int a, int b)->float {return a/b;});
    }
    
    virtual void exec(FloorDivNode* node, int l) {
        exec_element_wise_base(node, [](int a, int b)->float {return std::floor(a/b);});
    }
    
    virtual void exec(LessNode* node, int l) {
        exec_element_wise_base(node, [](int a, int b)->bool {return a < b;});
    }


};