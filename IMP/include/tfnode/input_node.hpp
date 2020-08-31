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
template<typename T>
class InputNode : public TensorNode<T> {
private:
public:
    InputNode(std::string name, Shape shape, std::string type) : 
        TensorNode<T>(name, shape, type) {}
    
    InputNode(const NDArray<T>& value, std::string name, Shape shape, std::string type) : 
        TensorNode<T>(value, name, shape, type) {}
};

template<typename T>
class ConstantNode : public InputNode<T> {
public:
    ConstantNode(const NDArray<T> &value, Shape shape, std::string name = "Const") :
        InputNode<T>(value, name, shape, "ConstantNode") {}
};

template<typename T>
class PlaceholderNode : public InputNode<T> {
public:
    PlaceholderNode(Shape shape, std::string name = "") : 
        InputNode<T>(name, shape, "PlaceholderNode") {}
};

template<typename T>
class VariableNode : public InputNode<T> {
public:
    VariableNode(NDArray<T> &value, Shape shape, std::string name = "") :
        InputNode<T>(value, name, shape, "VariableNode") {}
};


typedef ConstantNode<int> IntConstantNode;
typedef PlaceholderNode<int> IntPlaceholderNode;
typedef VariableNode<int> IntVariableNode; 
