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
    
    Tensor<float> nn;    
    for(auto i : range(0, K)) {
        nn.append(tf.math.argmax(ytr[indices[i]], 0));
    }

    auto nearest_neighbors = nn;

    auto y, idx, count = tf.unique_with_counts(nearest_neighbors);

    auto pred = tf.slice(y, begin=[tf.argmax(count, 0)], size=tf.constant([1], dtype=tf.int64))[0];
    accuracy=0

    init=tf.global_variables_initializer()

    //start of tensor session
    with tf.Session() as sess:

    for i in range(X_test.shape[0]):
        #return the predicted value
        predicted_value=sess.run(pred,feed_dict={xtr:X_traning,ytr:Y_traning,xte:X_test[i,:]})

        print("Test",i,"Prediction",predicted_value,"True Class:",np.argmax(Y_test[i]))

        if predicted_value == np.argmax(Y_test[i]):
            # if the prediction is right then a double value of 1./200 is added 200 here is the number of test
                accuracy += 1. / len(X_test)
    writer = tf.summary.FileWriter('./graphs',sess.graph)
    writer.close()
    # tensorboard --logdir="./graphs" --port 6006 in command promt to see the graph at localhost:6006
    print("Calculation completed ! ! ")
    print(K,"-th neighbors' Accuracy is:",accuracy)

    return 0;
}