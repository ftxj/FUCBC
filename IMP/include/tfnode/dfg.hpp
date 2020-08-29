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
    
    std::vector<BaseNode*> nodes;
public:
    DFG() : soruce_(new SourcesNode()) { }
    void BFS(Pass* func);
    void add_nodes(BaseNode* n) { nodes.push_back(n); }
    BaseNode& get_last_node() const { return *nodes[nodes.size() - 1]; }
    void add_input_node(InputNode *);
    SourcesNode* get_sources_node() const { return soruce_; }
    void print() const ;
};