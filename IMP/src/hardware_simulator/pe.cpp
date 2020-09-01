#include "hardware_simulator/memory_array.hpp"

std::vector<int> PE::exec_mul(std::vector<int> &in1, std::vector<bool> &mask) {
    std::vector<int> res(col_size, 0);
    for(size_t i = 0; i < col_size; ++i) {
        for(size_t j = 0; j < row_size; ++j) {
            if(mask[j] == true) {
                res[i] == matrix_[i][j] * in[j];
            }
        }
    }
    return shift_add(res);
}

std::vector<int> PE::exec_sum(std::vector<bool> &mask) {
    std::vector<int> cb_input(row_size, 0);
    for(size_t i = 0; i < mask.size(); ++i) {
        if(mask[i]) {
                cb_input[i] = 1;
        }
        else {
            cb_input[i] = 0;
        }
    }
    std::vector<int> res = dot(cb_input, std::vector<bool>(col_size_, true));
    return res;
}


std::vector<int> PE::exec_sub(std::vector<bool> &mask1, std::vector<bool> &mask2) {
    std::vector<int> cb_input(row_size, 0);
    for(size_t i = 0; i < mask.size(); ++i) {
        if(mask1[i]) {
            cb_input[i] = 1;
        }
        else if(mask2[i]) {
            cb_input[i] = -1;
        }
        else {
            cb_input[i] = 0;
        }
    }
    std::vector<int> res = dot(cb_input, std::vector<bool>(col_size_, true));
    return res;
}


std::vector<int> PE::exec_dot(std::vector<bool> &mask, std::vector<int> &input) {
    std::vector<int> cb_input(row_size, 0);
    for(size_t i = 0; i < mask.size(); ++i) {
        if(mask1[i]) {
            cb_input[i] = input[i];
        }
        else {
            cb_input[i] = 0;
        }
    }
    std::vector<int> res = dot(cb_input, std::vector<bool>(col_size_, true));
    return res;
}