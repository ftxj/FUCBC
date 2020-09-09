#include "tfAPI/tensorflow.hpp"
#include "data_structure/ndarray.hpp"
#include "data_structure/shape.hpp"
#include <string>
int main() {
    TensorFlow tf;
    Tensor<float> X;
    int k;
    auto start_point = tf.Variable<float>(X[]);
    auto centroids = tf.Variable<float>(start_pos.initialized_value(), 'S')

    auto mnist = tf.read_data_sets("data", true);

// #saving the datasets A.K.A Traning
// X_traning,Y_traning=mnist.train.next_batch(5000)
// X_test,Y_test=mnist.test.next_batch(200)

    auto xtr = tf.placeholder<float>(Shape(1024, 28 * 28));
    auto ytr = tf.placeholder<float>(Shape(1024, 10));
    auto xte = tf.placeholder<float>(Shape(128, 28 * 28)); 

    int K = 3;

    auto nearest_neighbors = tf.Variable(tf.zeros(Shape(K)));

    auto distance = tf.math.negative(
        tf.math.reduce_sum(
            tf.math.abs(
                tf.math.subtract(xtr, xte)),
                axis=1)
        );

    auto value_indices = tf.math.top_k(distance, K, false);
    auto indices = value_indices[1];
    auto value = value_indices[0];
    

    return 0;
}