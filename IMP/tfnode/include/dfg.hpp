#include<string>
#include<vector>
#include<map>
#include<set>

#include "base_node.h"

class SourcesNode : public BaseNode {

};

class SinkNode : public BaseNode {

};


class DFG {
private:
    SourcesNode* soruce;
    template<class T>
    void BFS(T func);
public:
    void print();
};