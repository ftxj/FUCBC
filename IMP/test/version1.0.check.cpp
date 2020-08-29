#include "tfAPI/tensorflow.hpp"
#include "data_structure/ndarray.hpp"
#include <string>
int main() {
    TensorFlow tf;
    auto x = tf.constant(3, "int32", Shape(1), "x");
    auto y = tf.constant(4, "int32", Shape(1), "y");
    auto A = tf.math.add(x, y, "add");

    auto x2 = tf.constant(3, "int32", Shape(1), "x2");
    auto y2 = tf.constant(4, "int32", Shape(1), "y2");
    auto A2 = tf.math.add(x2, y2, "add2");
    
    auto A3 = tf.math.add(A, A2, "add3");

    tf.run();
    return 0;
}