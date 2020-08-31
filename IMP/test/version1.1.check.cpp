#include "tfAPI/tensorflow.hpp"
#include "data_structure/ndarray.hpp"
#include <string>
int main() {
    TensorFlow tf;
    /*
        in1     in2     in3     in4     in5
          |-op1-|         |-op2-|          
              |-----op3-------|
                     |--op4---|
                                sink
    */
    auto in1 = tf.constant<int>(3, Shape(1), "in1");
    auto in2 = tf.constant<int>(4, Shape(1), "in2");
    auto op1 = tf.math.add(in1, in2, "add_op1");

    auto in3 = tf.placeholder<int>(Shape(36, 36), "in3");
    auto in4 = tf.constant<int>(5, Shape(36, 36), "in4");
    auto op2 = tf.math.subtract(in3, in4, "sub_op2");
    

    auto op3 = tf.math.div(op1, op2, "div_op3");
    
    auto op4 = tf.math.matmul(op1, op2, true, true, false, false, "matmul");
    

    tf.run();
    return 0;
}