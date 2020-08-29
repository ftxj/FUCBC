#include "optimization/print.hpp"
#include "tfnode/op_node.hpp"
#include "tfnode/base_node.hpp"
#include <iostream>

void Print::exec(BaseNode* node, int level) {
    std::string node_class = node->get_class();
    // if(first_meeting(node)) {
    //     std::cout << node->get_name() << 
    //         "[color=" << color_config[node_class] << ", \
    //           shape=" << shape_config[node_class] << ", \
    //           label=" << node->get_name() << "]\n";
        
    // }
    for(auto v : node->get_predecessors()) {
        std::cout << v->get_name() << "->" << node->get_name() << "\n"; 
    }
}