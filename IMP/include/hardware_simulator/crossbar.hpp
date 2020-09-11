#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>
#include "error/assert.hpp"
#include "hardware_simulator/digital.hpp"
#include "hardware_simulator/hardware_type.hpp"
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

    size_t ou_cols_;
    size_t ou_rows_;

public:

    CrossBar() : col_size_(128), row_size_(128), 
        cell_bits_(2), data_bits_(32), matrix_(128, AnalogBundle(128)) {}
    CrossBar(size_t size);
    
    CrossBar(Config &c) : matrix_(c.crossbar_rows_, AnalogBundle(c.crossbar_cols_)){
        col_size_ = c.crossbar_cols_;
        row_size_ = c.crossbar_rows_;
        cell_bits_ = c.crossbar_cell_width_;
        
        data_bits_ = c.crossbar_represent_number_bits_;
        
        ou_cols_ = c.crossbar_ou_cols_;
        ou_rows_ = c.crossbar_ou_rows_;
    }

    void dump(std::ostream &out) {
        bool out_empty = false;
        int out_empty_begin = 0;
        for(auto d : range(0, row_size_)) {
            if(!matrix_[d].is_zero()) {
                if(out_empty) {
                    out << d - out_empty_begin << "}\n"; 
                }
                matrix_[d].dump(out);
                out_empty = false;
            }
            else {
                if(!out_empty) {
                    out_empty_begin = d;
                    out << "{...";
                    out_empty = true;
                }
            }
        }
        if(out_empty) {
             out << row_size_ - out_empty_begin << "}"; 
        }
        out << "\n";
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

    void write_dummy(int col, int row, Analog_t value) {
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


    AnalogBundle dot(AnalogBundle &inp, int ou_col_index) {
        num_access_ += 1;
        //TODO use lambda replace loop nest
        AnalogBundle res(col_size_, 0);
        for(size_t i = ou_cols_ * ou_col_index; i < ou_cols_ * ou_col_index + ou_cols_; ++i) {
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
        assert_msg(matrix.size() == row_size_, "load data error" << matrix.size() << "!="<< row_size_);
        assert_msg(matrix[0].size() == col_size_, "load data error");
        for(int i = 0; i < row_size_; ++i) {
            matrix_[i] = matrix[i];
        }
    }
};