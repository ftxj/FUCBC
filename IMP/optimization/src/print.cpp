#include "print.hpp"
#include "op_node.h"
#include <iostream>

void Print::exec(BaseNode* node, int level) {
    string node_class = node->get_class();
    if(first_meeting(node)) {
        cout << node->get_name() << 
            "[color=" << color_config[node_class] << ", \
              shape=" << shape_config[node_class] << ", \
              label=" << node->get_name() << "]\n";
        
    }
    for(auto v : node->get_predecessors()) {
        cout << v->get_name() << "->" node->get_name() << "\n"; 
    }
}