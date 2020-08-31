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
    topological_order_.clear();
    
    std::vector<int> dfg_node_out_edges(nodes_vector_.size(), 0);
    std::vector<Node*> topo_order_nodes;
    std::vector<int> this_round_added;
    std::vector<int> in_ordered_masked(nodes_vector_.size(), 0);

    for(int i = 0; i < nodes_vector_.size(); ++i) {
        dfg_node_out_edges[i] = nodes_vector_[i]->get_predecessors().size();
        std::cout <<  nodes_vector_[i]->get_name() << "," << dfg_node_out_edges[i] << std::endl;
        if(dfg_node_out_edges[i] == 0) {
            this_round_added.push_back(i);
            topo_order_nodes.push_back(nodes_vector_[i]);
            topological_order_.push_back(0);
            in_ordered_masked[i] = 1;
        }
    }
    int round = 1;
    for(auto n : this_round_added) {
        std::cout << "round " << 0 << std::endl;
        std::cout << nodes_vector_[n]->get_name() << std::endl;
    }
    while(topo_order_nodes.size() != nodes_vector_.size()) {

        if(this_round_added.size() == 0) {
            assert_msg(0, "DFG with cycle, topo=" << topo_order_nodes.size() << ", DFG=" << nodes_vector_.size());
        }
        for(auto n : this_round_added) {
            for(auto succ : nodes_vector_[n]->get_successors()) {
                assert_msg(get_index(succ) >= 0, "node " << succ->get_name() << "not in vector");
                dfg_node_out_edges[get_index(succ)] --;
            }
        }
        this_round_added.clear();
        for(int i = 0; i < nodes_vector_.size(); ++i) {
            if(dfg_node_out_edges[i] == 0 && in_ordered_masked[i] == 0) {
                this_round_added.push_back(i);
                topo_order_nodes.push_back(nodes_vector_[i]);
                in_ordered_masked[i] = 1;
                topological_order_.push_back(round);
            }
        }
        for(auto n : this_round_added) {
            std::cout << "round " << round << std::endl;
            std::cout << nodes_vector_[n]->get_name() << std::endl;
        }
        round++;
    }
    nodes_vector_ = topo_order_nodes;
    has_topological_order_ = true;
}

int DFG::get_index(Node* n) {
    for(int i = 0; i < nodes_vector_.size(); ++i) {
        if(n == nodes_vector_[i]) {
            return i;
        }
    }
    return -1;
}


void DFG::add_node(Node* n, std::vector<Node*> pre_nodes) {
    std::cout << nodes_vector_.size() << std::endl;
    assert_msg(pre_nodes.size() > 0, "add node with no pre");
    assert_msg(get_index(n) == -1, "node already in vector");
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
    if(n->get_successors().size() == 0) {
        connect_with_sink(n);
    }
}

void DFG::replace_node(Node *n, Node *new_node) {
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

void DFG::delete_node(Node *n) {
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

bool DFG::is_connect_with_sink(Node* n) {
    for(auto succ_node : n->get_successors()) {
        if(succ_node == sink_) {
            return true;
        }
    }
    return false;
}

void DFG::connect_with_sink(Node* n) {
    n->add_successor(sink_);
    sink_->add_predecessors(n);
    has_topological_order_ = false;
}

void DFG::delete_connect_with_sink(Node* n) {
    for(auto succ_node : n->get_successors()) {
        if(succ_node == sink_) {
            n->delete_successor(sink_);
            sink_->delete_predecessors(n);
            break;
        }
    }
    has_topological_order_ = false;
}

bool DFG::is_connect_with_source(Node* n) {
    for(auto pre_node : n->get_predecessors()) {
        if(pre_node == soruce_) {
            return true;
        }
    }
    return false;
}

void DFG::connect_with_source(Node* n) {
    n->add_predecessors(soruce_);
    soruce_->add_successor(n);
    has_topological_order_ = false;
}

void DFG::delete_connect_with_source(Node* n) {
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
    pass->handle_header();
    for(int i = 0; i < topological_order_.size(); ++i) {
        pass->exec(nodes_vector_[i], topological_order_[i]);
    }
    pass->handle_tail();
}

void DFG::BFS(Pass* p) {
    std::queue<std::pair<Node*, int>> Q;
    std::map<Node*, bool> visited;

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