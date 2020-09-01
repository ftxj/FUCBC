#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>
#include "error/assert.hpp"
#include "hardware_simulator/digital.hpp"

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
    
    int num_access_;
    int num_access_rd_;
    int num_access_wr_;
    int latency_ip_;
    int latency_op_;
    int latency_rd_;
    int latency_wr_;


public:

    int get_input_lat() {
        return latency_ip_;
    }

    int get_output_lat() {
        return latency_op_;
    }

    int get_read_lat() {
        return latency_rd_;
    }

    int get_write_lat() {
        return latency_wr_;
    }

    std::vector<int> read_row(int row) {
        return matrix_t_[row];
    }

    void write(int col, int row, float value) {
        assert_msg(col < col_size, "col index outof bound");
        assert_msg(row < row_size, "row index outof bound");
        matrix_[row][col] = value;
        num_access_wr_ += 1;
    }

    float read(int col, int row) {
        assert_msg(col < col_size, "col index outof bound");
        assert_msg(row < row_size, "row index outof bound");
        num_access_rd_ += 1;
        return matrix_[row][col] ;
    }

    std::vector<float> dot(std::vector<float> &in) {
        num_access_ += 1;
        std::vector<float> res(col_size, 0);
        for(size_t i = 0; i < col_size; ++i) {
            for(size_t j = 0; j < row_size; ++j) {
                res[i] += matrix_[i][j] * in[j];
            }
        }
        return res;
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
};


class PE {
    CrossBar cross_bar_;
    DACArray rows_dac_array_;
    DACArray cols_dac_array_;
    SimpleHold sah_;
    ShiftAdd ssa_;
    ADC adc1_;
    ADC adc2_;
    RegFile pe_output_;
    RegFile pe_input_;
public:
    void exec_sum(std::vector<bool> &mask);
    void exec_sub(std::vector<bool> &mask1, std::vector<bool> &mask2);
    void exec_inner_product(std::vector<bool> &mask);
    void exec_outer_product();
    void exec_elewist_product(std::vector<bool> &mask);
};