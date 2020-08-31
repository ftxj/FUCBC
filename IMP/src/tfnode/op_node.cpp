#include "tfnode/op_node.hpp"

const std::map<std::string, int> BasicOp::op_code_map = {
        {"Abs", 1},         {"Add", 2},     {"ArgMin", 3},
        {"Div", 4},         {"Exp", 5},     {"FloorDiv",6},
        {"Less", 7},        {"Mul", 8},     {"RealDiv", 9},
        {"Sigmoid", 10},    {"Sqrt", 11},   {"Square", 12},
        {"Sub", 13},        {"Sum", 14},    {"Conv2D", 15},
        {"ExpandDims", 16}, {"MatMul", 17}, {"Reshape", 18},
        {"Tensordot", 19}
};

int generate_counter() {
        static int name_counter = 0;
        name_counter ++;
        return name_counter;
}

// void AbsNode::print() {

// }

// AddNode operator+(TensorNode& x, TensorNode& y) {
//     return 
// }

// AddNode* operator+(Tensor &x,  Tensor &y) {
//     assert(x.dtype_check(y));
//     assert(x.shape_equal(y));

//      TensorNode* a = static_cast< TensorNode*>(&x);
//      TensorNode* b = static_cast< TensorNode*>(&y);
//     //if(a != nullptr && b != nullptr) {
//         AddNode* node = new AddNode(a, b, "", a->get_shape(), a->get_dtype());
//         a->add_predecessors(node);
//         b->add_predecessors(node);
//         node->add_predecessors(a);
//         node->add_predecessors(b);
//         node->set_dfg(a->get_dfg());
//         return node;
//     //}
//     //return 
// }