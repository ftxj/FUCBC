#include "tensorflow.hpp"

int main() {
    TensorFlow tf;
    auto x = tf.constant<int>(3, , Shape(1));
    auto y = tf.constant<int>(4, , Shape(1));
    auto A = tf.math.add(x, y);
    tf.run();
    return 0;
}