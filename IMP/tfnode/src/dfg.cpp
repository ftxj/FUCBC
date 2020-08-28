#include "dfg.hpp"
#include <iostream>
#include <queue>
#include <functional>
#include "pass.hpp"
using namespace std;

void DFG::BFS(Pass p) {
    queue<pair<BaseNode*, int>> Q;
    int level = 0;
    Q.push({soruce, level});
    while(!Q.empty()) {
        auto {u, level} = Q.front();
        Q.pop();
        p.exec(u, level);
        for(auto v : u->get_predecessors()) {
            Q.push({v, level + 1});
        }
    }
};