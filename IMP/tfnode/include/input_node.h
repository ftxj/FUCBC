#include<string>
#include<vector>
#include<map>
#include<set>

#include "base_node.h"


class ConstNode;
class PlaceholderNode;
class VariableNode;
class Shape;
class Type;

template<typename T>
class InputNode : public BaseNode {
private:
    Shape shape_;
    Type dtype_;
public:
    InputNode(Shape shape, Type dtype, string name, string class_) : 
        BaseNode(name, class_, 0), 
        shape_(shape), 
        dtype_(dtype) {}

};

template<typename ValueType>
class ConstNode : public InputNode {
private:
    ValueType value_;
public:
    ConstNode(ValueType value, Type dtype, Shape shape, std::string name = "Const") :
        InputNode(shape, dtype, name, "ConstNode"), value_(value) {}
};

class PlaceholderNode : public BaseNode {
public:
    PlaceholderNode(Type dtype, Shape shape, std::string name = "") : 
        InputNode(shape, dtype, name, "Placeholder") {}
};

template<typename ValueType>
class VariableNode : public OpNode {
    ValueType initial_value_;
public:
    VariableNode(ValueType initial_value, Type dtype, Shape shape, std::string name = "Const") :
        InputNode(shape, dtype, name, "ConstNode"), value_(value) {}
};
