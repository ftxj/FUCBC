#include "tfnode/dfg.hpp"
#include <iostream>
#include <queue>
#include <functional>
#include "optimization/pass.hpp"
#include <map>


void DFG::topological_sort() {
    // TODO need test
    if(has_topological_order_) {
        return;
    }
    std::vector<int> dfg_node_out_edges(nodes_vector_.size(), 0);
    std::vector<BaseNode*> topo_order_nodes;
    
    std::vector<int> this_round_added;

    std::vector<int> in_ordered_masked(nodes_vector_.size(), 0);

    for(int i = 0; i < nodes_vector_.size(); ++i) {
        dfg_node_out_edges[i] = nodes_vector_[i]->get_successors().size();
        if(dfg_node_out_edges[i] == 0) {
            this_round_added.push_back(i);
            topo_order_nodes.push_back(nodes_vector_[i]);
            in_ordered_masked[i] = 1;
        }
    }
    while(topo_order_nodes.size() != nodes_vector_.size()) {
        if(this_round_added.size() == 0) {
            assert(0);
        }
        for(auto n : this_round_added) {
            for(auto succ : nodes_vector_[n]->get_successors()) {
                dfg_node_out_edges[get_index(succ)] --;
            }
        }
        this_round_added.clear();
        for(int i = 0; i < nodes_vector_.size(); ++i) {
            if(dfg_node_out_edges[i] == 0 && in_ordered_masked[i] == 0) {
                this_round_added.push_back(i);
                topo_order_nodes.push_back(nodes_vector_[i]);
            }
        }
    }
    has_topological_order_ = true;
}

void DFG::add_input_node(InputNode *node) {
    has_topological_order_ = false;
    connect_with_source(node);
    connect_with_sink(node);
    nodes_vector_.push_back(node);
}

void DFG::add_node(BaseNode* n, std::vector<BaseNode*> pre_nodes) {
    assert(pre_nodes.size() > 0);
    has_topological_order_ = false;
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
    has_topological_order_ = false;
    for(auto pre_node : n->get_predecessors()) {
        new_node->add_predecessors(pre_node);
        pre_node->add_successor(new_node);
    }
    for(auto succ_node : n->get_successors()) {
        new_node->add_successor(succ_node);
        succ_node->add_predecessors(new_node);
    }
    nodes_vector_.push_back(new_node);
    delete_node(n);
}

void DFG::delete_node(BaseNode *n) {
    for(auto pre_node : n->get_predecessors()) {
        pre_node->delete_successor(n);
    }
    for(auto succ_node : n->get_successors()) {
        succ_node->delete_predecessors(n);
    }
    for(auto iter = nodes_vector_.begin(); iter != nodes_vector_.end(); ++iter) {
        if(*iter == n) {
            nodes_vector_.erase(iter);
            break;
        }
    }
    has_topological_order_ = false;
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
    has_topological_order_ = false;
}

void DFG::delete_connect_with_sink(BaseNode* n) {
    for(auto succ_node : n->get_successors()) {
        if(succ_node == sink_) {
            n->delete_successor(sink_);
            sink_->delete_predecessors(n);
            break;
        }
    }
    has_topological_order_ = false;
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
    has_topological_order_ = false;
}

void DFG::delete_connect_with_source(BaseNode* n) {
    for(auto pre_node : n->get_predecessors()) {
        if(pre_node == soruce_) {
            n->delete_predecessors(soruce_);
            soruce_->delete_successor(n);
            break;
        }
    }
    has_topological_order_ = false;
}


void DFG::topological_pass(Pass* pass) {
    topological_sort();
    for(int i = 0; i < topological_order_.size(); ++i) {
        pass->exec(nodes_vector_[i], topological_order_[i]);
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