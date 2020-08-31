#include "tfAPI/tensorflow.hpp"
#include "data_structure/shape.hpp"

TensorFlow::TensorFlow() : math(TFMath()) {
    print = new Print();
    dfg_ = new DFG();
    math.set_dfg(dfg_);
}




//template<> ConstantNode<int>* TensorFlow::constant<int>(const NDArray<int> &value, Shape , std::string );    
// template<> PlaceholderNode<int>* TensorFlow::placeholder<int>(Shape shape, std::string );    
// template<> VariableNode<int>* TensorFlow::Variable<int>(const NDArray<int> &value, Shape shape, std::string);    
  
// template<> std::pair<TensorNode<int>*, TensorNode<int>*> TFMath::element_wise_shape_transfer<int, int>(Tensor<int> *x, Tensor<int> *y);
// template<> AbsNode<int>* TFMath::abs(Tensor<int> *, std::string);
// template<> ArgMinNode<int>* TFMath::argmin(Tensor<int> *, int , std::string, std::string);
// template<> ExpNode<int>* TFMath::exp(Tensor<int> *, std::string);
// template<> ExpandDimsNode<int>* TFMath::expand_dims(Tensor<int> *, int axis, std::string);
// template<> ReShapeNode<int>* TFMath::reshape(Tensor<int> *, Shape shape, std::string);
// template<> SigmoidNode<int>* TFMath::sigmod(Tensor<int> *, std::string);
// template<> SqrtNode<int>* TFMath::sqrt(Tensor<int> *, std::string);
// template<> SquareNode<int>* TFMath::square(Tensor<int> *, std::string);

// template<> AddNode<int>* TFMath::add(Tensor<int> *, Tensor<int> *y, std::string);

// template<> MatMulNode<int>* TFMath::matmul(Tensor<int> *a, Tensor<int> *b, 
//         bool , bool , bool , bool , std::string);
// template<> TensordotNode<int>* TFMath::tensordot(Tensor<int> *, Tensor<int> *, std::vector<int>,  std::string);

// template<> DivNode<int>* TFMath::div(Tensor<int> *, Tensor<int> *, std::string);
// template<> FloorDivNode<int>* TFMath::floordiv(Tensor<int> *, Tensor<int> *, std::string);
// template<> LessNode<int>* TFMath::less(Tensor<int> *, Tensor<int> *, std::string);
// template<> MulNode<int>* TFMath::multiply(Tensor<int> *, Tensor<int> *, std::string);
// template<> SubNode<int>* TFMath::subtract(Tensor<int> *, Tensor<int> *, std::string);
