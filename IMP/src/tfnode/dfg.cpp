#include "tfnode/dfg.hpp"
#include <iostream>
#include <queue>
#include <functional>
#include "optimization/pass.hpp"
#include <map>

void DFG::add_input_node(InputNode *node) {
    connect_with_source(node);
    connect_with_sink(node);
    nodes_vector_.push_back(node);
}

void DFG::add_node(BaseNode* n, std::vector<BaseNode*> pre_nodes) {
    assert(pre_nodes.size() > 0);
    nodes_vector_.push_back(n);
    if(is_connect_with_source(n)) {
        delete_connect_with_source(n);
    }
    for(auto pre_node : pre_nodes) {
        pre_node->add_successor(n);
        n->add_predecessors(pre_node);
        if(is_connect_with_sink(pre_node)) {
            delete_connect_with_sink(pre_node);
        }
    }
    if(n->get_predecessors().size() == 0) {
        connect_with_sink(n);
    }
}

void DFG::replace_node(BaseNode *n, BaseNode *new_node) {
    for(auto pre_node : n->get_predecessors()) {
        new_node->add_predecessors(pre_node);
        pre_node->add_successor(new_node);
    }
    for(auto succ_node : n->get_successors()) {
        new_node->add_successor(succ_node);
        succ_node->add_predecessors(new_node);
    }
    delete_node(n);
}

void DFG::delete_node(BaseNode *n) {
    for(auto pre_node : n->get_predecessors()) {
        pre_node->delete_successor(n);
    }
    for(auto succ_node : n->get_successors()) {
        succ_node->delete_predecessors(n);
    }
}

bool DFG::is_connect_with_sink(BaseNode* n) {
    for(auto succ_node : n->get_successors()) {
        if(succ_node == sink_) {
            return true;
        }
    }
    return false;
}

void DFG::connect_with_sink(BaseNode* n) {
    n->add_successor(sink_);
    sink_->add_predecessors(n);
}

void DFG::delete_connect_with_sink(BaseNode* n) {
    for(auto succ_node : n->get_successors()) {
        if(succ_node == sink_) {
            n->delete_successor(sink_);
            sink_->delete_predecessors(n);
            break;
        }
    }
}

bool DFG::is_connect_with_source(BaseNode* n) {
    for(auto pre_node : n->get_predecessors()) {
        if(pre_node == soruce_) {
            return true;
        }
    }
    return false;
}

void DFG::connect_with_source(BaseNode* n) {
    n->add_predecessors(soruce_);
    soruce_->add_successor(n);
}

void DFG::delete_connect_with_source(BaseNode* n) {
    for(auto pre_node : n->get_predecessors()) {
        if(pre_node == soruce_) {
            n->delete_predecessors(soruce_);
            soruce_->delete_successor(n);
            break;
        }
    }
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