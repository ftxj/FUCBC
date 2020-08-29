#pragma once
#include "tfnode/pass.hpp"
#include "tfnode/op_node.hpp"
#include "tfnode/input_node.hpp"
#include<string>
#include<vector>
#include<map>
#include<set>


class Evaluate : public Pass {
public:
    template<typename T>
    void exec(ConstNode<T>* node, int l) {

    } 

    template<typename T>
    void exec(AddNode<T>* node, int l) {
        for(auto v : )
    } 

};