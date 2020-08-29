#include "tfnode/op_node.hpp"

// void AbsNode::print() {

// }

// AddNode operator+(BaseNode& x, BaseNode& y) {
//     return 
// }

AddNode* operator+( Tensor &x,  Tensor &y) {
    assert(x.dtype_check(y));
    assert(x.shape_equal(y));

     BaseNode* a = static_cast< BaseNode*>(&x);
     BaseNode* b = static_cast< BaseNode*>(&y);
    //if(a != nullptr && b != nullptr) {
        AddNode* node = new AddNode(a, b, "", a->get_shape(), a->get_dtype());
        a->add_predecessors(node);
        b->add_predecessors(node);
        node->add_predecessors(a);
        node->add_predecessors(b);
        node->set_dfg(a->get_dfg());
        return node;
    //}
    //return 
}