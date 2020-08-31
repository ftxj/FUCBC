#pragma once
#include "tfnode/base_node.hpp"
#include "tfnode/op_node.hpp"
#include "optimization/pass.hpp"
#include <map>
#include <string>
#include <set>
class Print : public Pass {
private:
    std::set<Node*> node_meeting;
    
    std::map<std::string, std::string> color_config;
    std::map<std::string, std::string> fontcolor_config;
    std::map<std::string, std::string> shape_config;
    std::map<std::string, std::string> label_config;
    std::map<std::string, std::string> fillcolor_config;
    const std::string dot_pdf = "dot -ofile ";
    std::string number;
    bool first_meeting(Node* n);
public:
    #define VIRTUAL_EXEC_INT(type) virtual void exec(Int##type##Node*, int); 
    Print();
    void handle_header();
    void handle_tail();

    virtual void exec(Node*, int);
    VIRTUAL_EXEC_INT(Abs);
    VIRTUAL_EXEC_INT(Add);
    VIRTUAL_EXEC_INT(ArgMin);
    VIRTUAL_EXEC_INT(Div);
    VIRTUAL_EXEC_INT(Exp);
    VIRTUAL_EXEC_INT(FloorDiv);
    VIRTUAL_EXEC_INT(Less);
    VIRTUAL_EXEC_INT(Mul);
    //VIRTUAL_EXEC_INT(RealDiv);
    VIRTUAL_EXEC_INT(Sigmoid);
    VIRTUAL_EXEC_INT(Sqrt);
    VIRTUAL_EXEC_INT(Square);
    VIRTUAL_EXEC_INT(Sub);
    //VIRTUAL_EXEC_INT(Sum);
    //VIRTUAL_EXEC_INT(Conv2D);
    VIRTUAL_EXEC_INT(ExpandDims);
    VIRTUAL_EXEC_INT(MatMul);
    VIRTUAL_EXEC_INT(ReShape);
    VIRTUAL_EXEC_INT(Tensordot);
};