#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>
#include "error/assert.hpp"
#include "hardware_simulator/digital.hpp"
#include "hardware_simulator/crossbar.hpp"
#include "util.hpp"

class WordLineVectorGenerator {
    PrefixSum prefix_sum_;
    
    size_t ou_rows_;
public:
    DigitalBundle power_on(DigitalBundle &inp, size_t cycle) {
        DigitalBundle prefix_sum_res = prefix_sum_.power_on(inp);
        DigitalBundle res(inp.size());
        for(auto i : range(0, res.size())) {
            bool condition_1 = prefix_sum_res[i] >= (cycle - 1) * ou_rows_ + 1;
            bool condition_2 = prefix_sum_res[i] < cycle * ou_rows_ + 1;
            res[i] =  condition_1 & condition_2 & inp[i];
        }
        return res;
    }
};

class IndexDecoder {
private:
    PrefixSum prefix_sum_;
    size_t max_dist_;
    std::vector<std::vector<Digital_t>> input_index_; // ou index, ou1,ou2[1,3,4,5]; ou3,ou4[1,4,6,8]
    std::vector<Digital_t> prefix_sum_decode(std::vector<Digital_t> code) {
        return prefix_sum_.power_on(code);
    }
public:
    Digital_t power_on(size_t ou_index) {
        return prefix_sum_decode(input_index_[ou_index]);
    }
};


class PE {
private:
    CrossBar cross_bar_;
    DACArray rows_dac_array_;
    DACArray cols_dac_array_;
    SimpleHold sah_;

    std::vector<ShiftAdd> saa_;
    
    ADC adc1_;
    ADC adc2_;
    InputReg pe_input_;
    OutputReg pe_output_;
    
    IndexDecoder index_decoder_;
    WordLineVectorGenerator word_line_gen_;

    int cross_bar_cols_;
    int cross_bar_rows_;
    int cross_bar_cell_bits_;

    size_t store_num_bits_;

    size_t ou_cols_;
    size_t ou_rows_;

    size_t dac_res_;


public:

    PE() : 
        cross_bar_rows_(128), cross_bar_cols_(128), cross_bar_cell_bits_(2),
        store_num_bits_(16),
        ou_cols_(8), ou_rows_(8),
        dac_res(1)  {}
    Digital_t read_input(int pos) {
        return pe_input_.read(pos);
    }
    
    Digital_t read_output(int pos) {
        return pe_output_.read(pos);
    }

    std::vector<Digital_t> read_crossbar(int row) {
        auto row_data = cross_bar_.read_row(row);
        std::vector<Digital_t> res;
        for(auto i : range(0, row_data.size())) {
            res.push_back(adc1_.power_on(row_data[i]));
        }
        return res;
    }

    Digital_t read_crossbar(int row, int column) {
        auto row_data = cross_bar_.read_row(row);
        return adc1_.power_on(row_data[column]);
    }
    
    void write_input(int pos, Digital_t &d) {
        pe_input_.write(pos, d);
        //pe_input_.print();
    }
    void write_output(int pos, Digital_t &d) {
        pe_input_.write(pos, d);
    }

    void write_crossbar_dummy(int row, DigitalBundle &d) {
        assert_msg(d.size() == cross_bar_cols_, 
            "write to crossbar row must be " + std::to_string(cross_bar_cols_) + 
            "values, but give " + std::to_string(d.size()) + 
            "values, check your write instruction");
        assert_msg(d.signal_len_ == cross_bar_cell_bits_,
            "write to crossbar must be " + std::to_string(d.signal_len_) + 
            "bits, but give " + std::to_string(d.signal_len_) + 
            "bits, check your write instruction");
        std::vector<Analog_t> wd;
        for(auto x : d) {

        }
        cross_bar_.write_row_dummy(row,  wd);
    }

    // void write_crossbar(int row, int column, Digital_t &d) {
    //     cross_bar_.write(col, row, d);
    // }

    // void exec_sum(std::vector<bool> &mask);
    // void exec_sub(std::vector<bool> &mask1, std::vector<bool> &mask2);

    void exec_inner_product(std::vector<bool> &mask, int mul_bit = 16) {
        int round = mul_bit / rows_dac_array_.get_resolution();
        for(auto i : range(0, round)) {
            for(auto ou_col_index : range(0, cross_bar_cols_ / ou_cols_)) { 
                auto absolute_address = index_decoder_.power_on(ou_col_index);
                auto activation_value = pe_input_.read(absolute_address);
                for(auto ou_row_index : range(0, ou_rows_)) {
                    auto activation_vector = word_line_gen_.power_on(activation_value, ou_row_index);
                    auto dac_out = rows_dac_array_.power_on(activation_vector);
                    auto cb_out = cross_bar_.dot(dac_out, ou_col_index);
                    auto sah_out = sah_.power_on(cb_out);
                    for(auto col_index : range(ou_col_index * ou_cols_, ou_col_index * ou_cols_ + ou_cols_)) {
                        auto adc_out = adc1_.power_on(sah_out[col_index]));
                        saa_[col_index].power_on(adc_out, i * dac_res_);
                    }
                }
            }
        }
        size_t number_of_packaged_result = (cross_bar_cols_ * cross_bar_cell_bits_) / store_num_bits_; 
        size_t number_of_cols_per_result = store_num_bits_ / cross_bar_cell_bits_;
        for(auto out_solt : range(0, number_of_packaged_result) {
            size_t elected_ssa_index = out_solt * number_of_cols_per_result;
            for(auto index : range(1,  number_of_cols_per_result + 1)) {
                saa_[elected_ssa_index].power_on(saa_[elected_ssa_index + index].get_data(), cross_bar_cell_bits_ * index);
            }
            pe_output_.write(out_solt, saa_[elected_ssa_index].get_data());
        }
    }

    // void exec_outer_product();
    // void exec_elewist_product(std::vector<bool> &mask);
};