#include<string>
#include<vector>
#include<map>
#include<set>

#include "tfnode/base_node.hpp"
#include "tfnode/input_node.hpp"


class SourcesNode  {
    std::vector<BaseNode*> successors_;
    std::vector<BaseNode*>& add_successor(BaseNode* node) { successors_.push_back(node); }
};


class SinkNode  {

};


class DFG {
private:
    SourcesNode* soruce;
    template<class T>
    void BFS(T func);
public:
    template<typename T>
    void add_input_node(InputNode<T> *);
    SourcesNode* get_sources_node() { return soruce; }
    void print();
};