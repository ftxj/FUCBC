#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>

#include "tfnode/base_node.hpp"
#include "data_structure/ndarray.hpp"
//#include "../../util.hpp"

// class ConstNode;
// class PlaceholderNode;
// class VariableNode;
// class Shape;
// class Type;

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

// class PlaceholderNode : public BaseNode {
// public:
//     PlaceholderNode(Type dtype, Shape shape, std::string name = "") : 
//         InputNode(shape, dtype, name, "Placeholder") {}
// };

// template<typename ValueType>
// class VariableNode : public OpNode {
//     ValueType initial_value_;
// public:
//     VariableNode(ValueType initial_value, Type dtype, Shape shape, std::string name = "Const") :
//         InputNode(shape, dtype, name, "ConstNode"), value_(value) {}
// };
