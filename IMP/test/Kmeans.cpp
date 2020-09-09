#include "tfAPI/tensorflow.hpp"
#include "data_structure/ndarray.hpp"

#include <string>
int main() {
    TensorFlow tf;
    Tensor<float> X;
    int k;
    auto start_point = tf.Variable<float>(X[]);
    auto centroids = tf.Variable<float>(start_pos.initialized_value(), 'S')

    auto points = tf.Variable(X, "X");
    auto ones_like = tf.ones(Shape(points.get_shape()[0], 1));
    auto prev_assignments = tf.Variable<int>(tf.zeros((points.get_shape()[0], )));

    auto p1 = tf.math.matmul(
                tf.math.expand_dims(
                    tf.math.reduce_sum(
                        tf.math.square(points), 1), 1),
                tf.ones(Shape(1, k))
            );

    auto p2 = tf.math.transpose(
            tf.matmul(
                tf.math.reshape(tf.math.reduce_sum(tf.math.square(centroids), 1), Shape(-1, 1)),
                ones_like, false, true)
            );
    
    auto distance = tf.math.sqrt(tf.math.add(p1, p2) - 2 * tf.math.matmul(points, centroids, false, true));

    auto point_to_centroid_assignment = tf.math.argmin(distance, 1);

    auto total = tf.math.unsorted_segment_sum(points, point_to_centroid_assignment, k);
    auto count = tf.math.unsorted_segment_sum(ones_like, point_to_centroid_assignment, k);
    auto means = total / count;

    auto is_continue = tf.math.reduce_any(tf.math.not_equal(point_to_centroid_assignment, prev_assignments));

    tf.run(tf.global_variables_initializer());
    return 0;
}