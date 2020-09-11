#include <vector>
#include "error/assert.hpp"
template<typename T, typename T2>
std::vector<T> baseline_dot(std::vector<std::vector<T2>> &matrix, std::vector<T> &inp) {
    assert_msg(matrix.size() > 0, "wuwwu ");
    assert_msg(matrix.size() <= inp.size(), "wuwwu ");
    std::vector<T> res(matrix[0].size());
    for(int i = 0; i < matrix.size(); ++i) {
        for(int j = 0; j < matrix[0].size(); ++j) {
            res[j] += matrix[i][j] * inp[i];
        }
    }
    return res;
}

void fill_int_vector_with_random_number(std::vector<int> &vec, int max) {
    auto gen = [&max](){
        return static_cast <int> (rand()) / (static_cast <int> (RAND_MAX/max));
    };
    std::generate(std::begin(vec), std::end(vec), gen);
}


void fill_int_vector_with_random_number(std::vector<int>::iterator &begin, std::vector<int>::iterator &end, int max) {
    auto gen = [&max](){
        return static_cast <int> (rand()) / (static_cast <int> (RAND_MAX/max));
    };
    std::generate(begin, end, gen);
}



void fill_vector_with_random_number(std::vector<float> &vec, float max) {
    auto gen = [&max](){
        return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/max));
    };
    std::generate(std::begin(vec), std::end(vec), gen);
}



void fill_matrix_with_random_number(std::vector<std::vector<float>> &matrix, float max) {
    for(int i = 0; i < matrix.size(); ++i) {
        fill_vector_with_random_number(matrix[i], max);
    }
}
