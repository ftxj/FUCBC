#include "optimization/print.hpp"
#include "tfnode/op_node.hpp"
#include "tfnode/base_node.hpp"
#include <iostream>

#define CONFIG_COLOR(a,b) color_config[a]=b 
#define CONFIG_SHAPE(a,b) color_config[a]=b 
Print::Print(){
    CONFIG_COLOR("ConstNode","darkgoldenrod1");
    CONFIG_COLOR("PlaceholderNode","cadetblue2");
    CONFIG_COLOR("VariableNode","gray80");

    CONFIG_COLOR("AbsNode","darkgoldenrod1");
    CONFIG_COLOR("ExpNode","cadetblue2");
    CONFIG_COLOR("SigmodNode","gray80");
    CONFIG_COLOR("SqrtNode","darkgoldenrod1");
    CONFIG_COLOR("SquareNode","cadetblue2");
    CONFIG_COLOR("ReShapeNode","gray80");
    CONFIG_COLOR("TransposeNode","darkgoldenrod1");
    CONFIG_COLOR("ArgMinNode","cadetblue2");
    CONFIG_COLOR("ExpandDims","gray80");

    CONFIG_COLOR("AddNode","indigo");
    
    CONFIG_COLOR("DivNode","cadetblue2");
    CONFIG_COLOR("FloorDivNode","gray80");
    CONFIG_COLOR("LessNode","cadetblue2");
    CONFIG_COLOR("MulNode","gray80");
    CONFIG_COLOR("SubNode","darkgoldenrod1");
    CONFIG_COLOR("MatMulNode","cadetblue2");
    CONFIG_COLOR("TensordotNode","gray80");
    CONFIG_COLOR("SinkNode","white");
    CONFIG_COLOR("SourcesNode","white");
}

bool Print::first_meeting(Node* n) {
    if(node_meeting.find(n) == node_meeting.end()) {
        node_meeting.insert(n);
        return true;   
    }
    return false;
}

void Print::handle_header() {
    std::cout << "digraph dfg{" << std::endl;
    std::cout << "graph [label=\"DFG\", stype = filled];" << std::endl;
}

void Print::handle_tail() {
    std::cout << "}" << std::endl;
}

void Print::exec(Node* node, int level) {
    //node->print(levle);
    std::string node_class = node->get_class();
    if(first_meeting(node)) {
        std::cout << node->get_name() << 
            " [style=filled, fillcolor=\"" << color_config[node_class] << "\", label=\"" \
            << node->get_name() + "\\n" + node->get_class().substr(0, node->get_class().size() - 4) << "\"];\n";
              //shape=" << shape_config[node_class] << ", 
    }
    for(auto v : node->get_predecessors()) {
        std::cout << v->get_name() << "->" << node->get_name() << "\n"; 
    }
}

#define EXEC_INT_FUNCRION(type) void Print::exec(Int##type##Node* u, int l) { exec(static_cast<Node*>(u), l); } 

EXEC_INT_FUNCRION(Abs);
EXEC_INT_FUNCRION(Add);
EXEC_INT_FUNCRION(ArgMin);
EXEC_INT_FUNCRION(Div);
EXEC_INT_FUNCRION(Exp);
EXEC_INT_FUNCRION(FloorDiv);
EXEC_INT_FUNCRION(Less);
EXEC_INT_FUNCRION(Mul);
//EXEC_INT_FUNCRION(RealDiv);
EXEC_INT_FUNCRION(Sigmoid);
EXEC_INT_FUNCRION(Sqrt);
EXEC_INT_FUNCRION(Square);
EXEC_INT_FUNCRION(Sub);
//EXEC_INT_FUNCRION(Sum);
//EXEC_INT_FUNCRION(Conv2D);
EXEC_INT_FUNCRION(ExpandDims);
EXEC_INT_FUNCRION(MatMul);
EXEC_INT_FUNCRION(ReShape);
EXEC_INT_FUNCRION(Tensordot);