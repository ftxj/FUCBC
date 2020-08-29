#pragma once
#include "tfnode/op_node.hpp"
#include "tfnode/input_node.hpp"
#include "tfnode/dfg.hpp"
#include "optimization/print.hpp"
#include "data_structure/shape.hpp"
#include <iostream>
class TFMath {
private:
    std::string dtype_check(Tensor *x, Tensor *y);
    Shape shape_check(Tensor *x, Tensor *y);
    DFG* dfg_check(BaseNode *x, BaseNode *y);
public:
    // AbsNode abs(Tensor x, std::string name = "");
    AddNode* add(Tensor *x, Tensor *y, std::string name = "");
};

class TensorFlow {
public:
    Print* print;
    DFG* dfg_;

    TFMath math;
    TensorFlow();

    //template<typename ValueType>
    //VariableNode<ValueType> Variable(NDArray<ValueType> &initial_value, Type dtype, Shape shape, std::string name = "");

    ConstantNode* constant(NDArray value, std::string dtype = "unknow", Shape shape = Shape(), std::string name = "Const");    

    //PlaceholderNode placeholder(Type dtype, Shape shape, std::string name = "");

    void run();
};