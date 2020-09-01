#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>
#include "error/assert.hpp"
#include "hardware_simulator/digital.hpp"

class Conductance_t {
public:
    operator Analog_t(); // TODO
    void operator= (Analog_t);
    operator= (Conductance_t);
};

class CrossBar {
private:
    std::vector<std::vector<Conductance_t>> matrix_;
    std::vector<std::vector<Conductance_t>> matrix_t_;

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
    CrossBar();
    CrossBar(size_t size);
    CrossBar(Config);
    
    int get_input_lat() { return latency_ip_; }
    int get_output_lat() { return latency_op_; }
    int get_read_lat() { return latency_rd_; }
    int get_write_lat() { return latency_wr_; }

    std::vector<Analog_t> read_row(int row) { return matrix_t_[row]; }

    void write(int col, int row, Analog_t value) {
        assert_msg(col < col_size, "col index outof bound");
        assert_msg(row < row_size, "row index outof bound");
        matrix_[row][col] = value;
        num_access_wr_ += 1;
    }

    Analog_t read(int col, int row) {
        assert_msg(col < col_size, "col index outof bound");
        assert_msg(row < row_size, "row index outof bound");
        num_access_rd_ += 1;
        return matrix_[row][col] ;
    }

    std::vector<Analog_t> dot(std::vector<Analog_t> &inp) {
        num_access_ += 1;
        std::vector<Analog_t> res(col_size, 0);
        for(size_t i = 0; i < col_size; ++i) {
            for(size_t j = 0; j < row_size; ++j) {
                res[i] += matrix_[i][j] * inp[j];
            }
        }
        return res;
    }
};