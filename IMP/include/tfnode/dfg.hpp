#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>

#include "tfnode/base_node.hpp"
#include "tfnode/input_node.hpp"
#include "optimization/pass.hpp"

class DFG {
private:
    SourcesNode* soruce_;
    SinkNode* sink_;
    
    std::vector<Node*> nodes_vector_;
    std::vector<int> topological_order_;
    bool has_topological_order_;

    void topological_sort();
    int get_index(Node* n);

    void delete_node(Node* n);
public:
    DFG() : soruce_(new SourcesNode()), sink_(new SinkNode()) {
        nodes_vector_.push_back(soruce_);
        nodes_vector_.push_back(sink_);
    }

    void add_node(Node* n, std::vector<Node*> pre_nodes);

    template<typename T>
    void add_input_node(InputNode<T>* n);

    void replace_node(Node* n, Node *new_node);

    bool is_connect_with_sink(Node* n);
    void connect_with_sink(Node* n);
    void delete_connect_with_sink(Node* n);

    bool is_connect_with_source(Node *);
    void connect_with_source(Node *);
    void delete_connect_with_source(Node* n);

    SourcesNode* get_sources_node() const { return soruce_; }
    SinkNode* get_sink_node() const { return sink_; }
    
    void topological_pass(Pass* pass);
    void BFS(Pass* func);
};

template<typename T>
void DFG::add_input_node(InputNode<T> *node) {
    has_topological_order_ = false;
    connect_with_source(node);
    connect_with_sink(node);
    nodes_vector_.push_back(node);
}
