#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>

#include "tfnode/base_node.hpp"
#include "data_structure/ndarray.hpp"
//#include "../../util.hpp"

// class InputNode; 
// class ConstantNode;
// class PlaceholderNode;
// class VariableNode;

class InputNode : public BaseNode {
private:
public:
    InputNode(std::string name, Shape shape, std::string dtype, std::string type) : 
        BaseNode(name, shape, dtype, type) {}
    
    InputNode(NDArray& value, std::string name, Shape shape, std::string dtype, std::string type) : 
        BaseNode(value, name, shape, dtype, type) {}
};

class ConstantNode : public InputNode {
public:
    ConstantNode(NDArray &value, std::string dtype, Shape shape, std::string name = "Const") :
        InputNode(value, name, shape, dtype, "ConstNode") {}
};

class PlaceholderNode : public InputNode {
public:
    PlaceholderNode(std::string dtype, Shape shape, std::string name = "") : 
        InputNode(name, shape, dtype, "PlaceholderNode") {}
};

class VariableNode : public InputNode {
public:
    VariableNode(NDArray &value, std::string dtype, Shape shape, std::string name = "") :
        InputNode(value, name, shape, dtype, "VariableNode") {}
};
