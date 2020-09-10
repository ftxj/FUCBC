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
    std::vector<AnalogBundle> matrix_;
    std::vector<AnalogBundle> matrix_t_;
    // unit column, 对输入求和
    // bit map column, 指示哪一个是 flip存储
    // ISAAC，预先预测，这个column的最大输出会不不导致MSB是1，如果会，那就flip他
    size_t col_size_;
    size_t row_size_;
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

    CrossBar() : col_size_(128), row_size_(128), 
        cell_bits_(2), data_bits_(32), matrix_(128, AnalogBundle(128)) {}
    CrossBar(size_t size);
    CrossBar(Config);

    void dunmp(std::ostream &out) {
        out << "\n";
        for(auto i : matrix_) {
            for(auto j : i) {
                j.print();
                out << " ";    
            }
            out << "\n";
        }
    }
    void clear() {
        for(auto i : range(0, row_size_)) {
            matrix_[i] = AnalogBundle(col_size_);
        }
    }

    int get_input_lat() { return latency_ip_; }
    int get_output_lat() { return latency_op_; }
    int get_read_lat() { return latency_rd_; }
    int get_write_lat() { return latency_wr_; }

    AnalogBundle read_row(int row) { return matrix_t_[row]; }

    void write(int col, int row, Analog_t value) {
        assert_msg(col < col_size_, "col index outof bound");
        assert_msg(row < row_size_, "row index outof bound");
        matrix_[row][col] = value;
        num_access_wr_ += 1;
    }

    void write_row(int row, AnalogBundle value) {
        //assert_msg(col < col_size, "col index outof bound");
        assert_msg(row < row_size_, "row index outof bound");
        assert_msg(value.size() == col_size_, "write analog out of bound");
        matrix_[row] = value;
        num_access_wr_ += 1;
    }


    void write_row_dummy(int row, AnalogBundle value) {
        //assert_msg(col < col_size, "col index outof bound");
        assert_msg(row < row_size_, "row index outof bound");
        assert_msg(value.size() == col_size_, "write analog out of bound");
        matrix_[row] = value;
        num_access_wr_ += 1;
    }


    Analog_t read(int col, int row) {
        assert_msg(col < col_size_, "col index outof bound");
        assert_msg(row < row_size_, "row index outof bound");
        num_access_rd_ += 1;
        return matrix_[row][col] ;
    }

    AnalogBundle read(int row) {
        assert_msg(row < row_size_, "row index outof bound");
        num_access_rd_ += 1;
        return matrix_[row] ;
    }

    AnalogBundle dot(AnalogBundle &inp) {
        num_access_ += 1;
        //TODO use lambda replace loop nest
        AnalogBundle res(col_size_, 0);
        for(size_t i = 0; i < col_size_; ++i) {
            for(size_t j = 0; j < row_size_; ++j) {
                res[i] += matrix_[j][i] * inp[j];
            }
        }
        return res;
    }

    void load_data(std::vector<AnalogBundle> matrix) {
        matrix_ = matrix;
    }
    
    void load_data(std::vector<std::vector<float>> matrix) {
        for(int i = 0; i < row_size_; ++i) {
            matrix_[i] = matrix[i];
        }
    }
};