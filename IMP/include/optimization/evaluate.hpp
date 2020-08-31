// #pragma once
// #include "tfnode/pass.hpp"
// #include "tfnode/op_node.hpp"
// #include "tfnode/input_node.hpp"
// #include "tfnode/base_node.hpp"

// #include<string>
// #include<vector>
// #include<map>
// #include<set>
// #include <cmath>


// class Evaluate : public Pass {
// private:
//     template<class T1, class T2>
//     void exec_element_wise_base(TensorNode<T1>* node, std::function<T2 (T1, T1)> op) {
//         std::vector<Node*> pred = node->get_predecessors();
//         assert_msg(pred.size() == 2, "element wise op must have two pre");
//         TensorNode<T1>* x = static_cast<TensorNode<T1>*>(pred[0]);
//         TensorNode<T2>* x = static_cast<TensorNode<T1>*>(pred[1]);
//         NDArray<T2> res = x->get_values().element_wist_op(y->get_values(), op);
//         node->set_values(res);
//     }

//     template<class T1, class T2>
//     void exec_unary_op_base(TensorNode<T1>* node, std::function<T2 (T1)> op) {
//         std::vector<Node*> pred = node->get_predecessors();
//         assert_msg(pred.size() == 1, "unary op must have one pre");
//         TensorNode<T1>* x = static_cast<TensorNode<T1>*>(pred[0]);
//         NDArray<T2> res = x->get_values().unary_op(op);
//         node->set_values(res);
//     }
// public:
//     void exec(ConstNode* node, int l) {

//     }

//     template<T> void exec(AbsNode<T>* node, int l) {
//         exec_unary_op_base(node, [](T a)->T {return a > 0? a : -a;});
//     }

//     template<T> void exec(ExpNode<T>* node, int l) { 
//         exec_unary_op_base(node, [](T a)->float {return std::exp(a);});
//     }

//     template<T> void exec(SqrtNode<T>* node, int l) { 
//         exec_unary_op_base(node, [](T  a)->float {return std::sqrt(a);});
//     }

//     template<T> void exec(SquareNode<T>* node, int l) { 
//         exec_unary_op_base(node, [](T  a)->float {return std::pow(a, 2);});
//     }

//     template<T> void exec(SigmoidNode<T>* node, int l) { 
//         exec_unary_op_base(node, [](T  a)->float {return a/ (1+(a>0?a:-a));});
//     }


//     //template<T> void exec(TransposeNode<T>* node, int l) { exec(static_cast<TensorNode*>(u), l); }
//     //template<T> void exec(ArgMinNode<T>* node, int l) { exec(static_cast<TensorNode*>(u), l); }
  

//     template<T> void exec(AddNode<T>* node, int l) {
//         exec_element_wise_base(node, [](T  a, T  b)->T  {return a+b;});
//     }

//     template<T> void exec(SubNode<T>* node, int l) {
//         exec_element_wise_base(node, [](T  a, T  b)->T  {return a-b;});
//     }
    
//     template<T> void exec(MulNode<T>* node, int l) {
//         exec_element_wise_base(node, [](T  a, T  b)->T  {return a*b;});
//     }
    
//     template<T> void exec(DivNode<T>* node, int l) {
//         exec_element_wise_base(node, [](T  a, T  b)->float {return a/b;});
//     }
    
//     template<T> void exec(FloorDivNode<T>* node, int l) {
//         exec_element_wise_base(node, [](T  a, T  b)->float {return std::floor(a/b);});
//     }
    
//     template<T> void exec(LessNode<T>* node, int l) {
//         exec_element_wise_base(node, [](T  a, T  b)->bool {return a < b;});
//     }


// };