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
    std::vector<BaseNode*> nodes_vector_;
    bool has_topological_order_;

    void delete_node(BaseNode* n);
public:
    DFG() : soruce_(new SourcesNode()), sink_(new SinkNode()) { }


    void add_node(BaseNode* n, std::vector<BaseNode*> pre_nodes);
    void add_input_node(InputNode* n);
    void replace_node(BaseNode* n, BaseNode *new_node);

    bool is_connect_with_sink(BaseNode* n);
    void connect_with_sink(BaseNode* n);
    void delete_connect_with_sink(BaseNode* n);

    bool is_connect_with_source(BaseNode *);
    void connect_with_source(BaseNode *);
    void delete_connect_with_source(BaseNode* n);

    SourcesNode* get_sources_node() const { return soruce_; }
    SinkNode* get_sink_node() const { return sink_; }
    
    void BFS(Pass* func);
};