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
    void exec(ConstNode* node, int l) {

    } 

    void exec(AddNode* node, int l) {
    } 

};