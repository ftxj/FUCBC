#include "tfAPI/tensorflow.hpp"
#include "data_structure/ndarray.hpp"
#include <string>
int main() {
    TensorFlow tf;
    auto in1 = tf.constant(3, "int32", Shape(1), "in1");
    auto in2 = tf.constant(4, "int32", Shape(1), "in2");
    auto op1 = tf.math.add(in1, in2, "add_op1");

    auto in3 = tf.placeholder("int32", Shape(36, 36), "in3");
    auto in4 = tf.constant(4, "int32", Shape(36, 36), "in4");
    auto op2 = tf.math.subtract(in3, in4, "sub_op2");
    

    auto op3 = tf.math.div(op1, op2, "div_op3");
    
    auto in5 = tf.placeholder("int32", Shape(36, 36), "in5");
    auto op4 = tf.math.matmul(op2, op3, true, true, false, false, "matmul_op4");

    tf.run();
    return 0;
}