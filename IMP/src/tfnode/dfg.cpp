#include "tfnode/dfg.hpp"
#include <iostream>
#include <queue>
#include <functional>
#include "optimization/pass.hpp"
#include <map>

void DFG::add_input_node(InputNode *node) {
    soruce_->add_successor(node);
    node->add_predecessors(soruce_);
}


void DFG::BFS(Pass* p) {
    std::queue<std::pair<BaseNode*, int>> Q;
    std::map<BaseNode*, bool> visited;

    int level = 0;
    Q.push({soruce_, level});
    while(!Q.empty()) {
        auto pair = Q.front();
        auto u = pair.first;
        level = pair.second;
        Q.pop();
        p->exec(u, level);
        for(auto v : u->get_successors()) {
            if(visited.find(v) == visited.end()) {
                Q.push({v, level + 1});
                visited[v] = true;
            }
        }
    }
};