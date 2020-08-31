#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>


// class DAC {
// private:
//     static double analog_max;
// public:
//     static double propagate(int inp);
// };

// class DACArray {
// public:
//     static std::vector<double> propagate(vector<int> inp) {
//     }
// };


// class ADC {
  
// };

// class Reg {

// };

class CrossBar {
private:
    typedef float conductance_t;
    typedef float analog_t; 

    std::vector<std::vector<conductance_t>> matrix_;
    std::vector<std::vector<conductance_t>> matrix_t_;
    size_t col_size;
    size_t row_size;
    size_t cell_bits_;
    size_t data_bits_;

public:

    std::vector<int> read_row(int row) {
        return matrix_t_[row];
    }

    std::vector<int> read_from_address(LocalAddress &src) {
        return matrix_t_[row];
    }

    std::vector<int> shift_add(std::vector<int> &in) {
        size_t cols_per_data = data_bits_ / cell_bits_;
        std::vector<int> res(in.size() / cols_per_data, 0);
        for(size_t i = 0; i < in.size(); i + =cols_per_data){
            for(size_t j = 0; j < cols_per_data; ++j) {
                res[i] += in[i * cols_per_data + j] << (j * cell_bits_);
            }
        }
        return res;
    }

    std::vector<int> dot(std::vector<int> &in, std::vector<bool> &mask) {
        std::vector<int> res(col_size, 0);
        for(size_t i = 0; i < col_size; ++i) {
            if(mask[i] == true) {
                for(size_t j = 0; j < row_size; ++j) {
                    res[i] += matrix_[i][j] * in[j];
                }
            }
        }
        return shift_add(res);
    }

    std::vector<int> exec_mul(std::vector<int> &in1, std::vector<bool> &mask) {
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

    std::vector<int> exec_sum(std::vector<bool> &mask) {
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


    std::vector<int> exec_sub(std::vector<bool> &mask1, std::vector<bool> &mask2) {
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


    std::vector<int> exec_dot(std::vector<bool> &mask, std::vector<int> &input) {
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
};