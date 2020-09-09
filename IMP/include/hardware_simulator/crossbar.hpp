#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>
#include "error/assert.hpp"
#include "hardware_simulator/digital.hpp"
#include <iomanip>

class CrossBar {
private:
    std::vector<std::vector<Analog_t>> matrix_;
    std::vector<std::vector<Analog_t>> matrix_t_;
    // unit column, 对输入求和
    // bit map column, 指示哪一个是 flip存储
    // ISAAC，预先预测，这个column的最大输出会不不导致MSB是1，如果会，那就flip他
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

    CrossBar() : col_size(128), row_size(128), 
        cell_bits_(2), data_bits_(32), matrix_(128, std::vector<Analog_t>(128)) {}
    CrossBar(size_t size);
    CrossBar(Config);

    void print() {
        std::cout << "\n";
        for(auto i : matrix_) {
            for(auto j : i) {
                j.print();
                std::cout << " ";    
            }
            std::cout << "\n";
        }
    }
    
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

    void write_row(int row, std::vector<Analog_t> value) {
        //assert_msg(col < col_size, "col index outof bound");
        assert_msg(row < row_size, "row index outof bound");
        assert_msg(value.size() == col_size, "write analog out of bound");
        assert_msg(matrix_.size() == row_size, "write analog out of bound");
        assert_msg(matrix_[0].size() == col_size, "write analog out of bound");
        matrix_[row] = value;
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