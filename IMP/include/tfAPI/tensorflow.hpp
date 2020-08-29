#pragma once
#include "tfnode/op_node.hpp"
#include "tfnode/input_node.hpp"
#include "tfnode/dfg.hpp"
#include "optimization/print.hpp"
#include <iostream>
class TFMath {
public:
    // AbsNode abs(Tensor x, std::string name = "");
    
    AddNode* add( Tensor *x,  Tensor *y, std::string name = "") {
        assert(x->dtype_check(*y));
        assert(x->shape_equal(*y));

        BaseNode* a = static_cast< BaseNode*>(x);
        BaseNode* b = static_cast< BaseNode*>(y);
        //if(a != nullptr && b != nullptr) {
        AddNode* node = new AddNode(a, b, name, a->get_shape(), a->get_dtype());
        a->add_successor(node);
        b->add_successor(node);
        node->add_predecessors(a);
        node->add_predecessors(b);
        node->set_dfg(a->get_dfg());
        return node;
    }
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