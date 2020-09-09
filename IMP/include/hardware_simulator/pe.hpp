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
    std::vector<Digital_t> power_on(std::vector<Digital_t> &inp, size_t cycle) {
        std::vector<Digital_t> prefix_sum_res = prefix_sum_.power_on(inp);
        std::vector<Digital_t> res(inp.size());
        for(auto i : range(0, res.size())) {
            bool condition_1 = prefix_sum_res[i] >= (cycle - 1) * ou_rows_ + 1;
            bool condition_2 = prefix_sum_res[i] < cycle * ou_rows_ + 1;
            rez[i] =  condition_1 & condition_2 & inp[i];
        }
    };
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
    };
};


class PE {
private:
    CrossBar cross_bar_;
    DACArray rows_dac_array_;
    DACArray cols_dac_array_;
    SimpleHold sah_;
    ShiftAdd saa_;
    ADC adc1_;
    ADC adc2_;
    InputReg pe_input_;
    OutputReg pe_output_;
    
    IndexDecoder index_decoder_;
    WordLineVectorGenerator word_line_gen_;

    int cross_bar_cols_;
    int cross_bar_rows_;
    int bit_per_cell_;
    int simd_solt_;

public:

    PE() : simd_solt_(8), cross_bar_rows_(128), cross_bar_cols_(128), bit_per_cell_(2)  {}
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
    void write_crossbar(int row, std::vector<Digital_t> &d) {
        
        auto wd = cols_dac_array_.power_on(d);
        for(auto i : wd) {
            i.print();
            std::cout << " ";
        }
        std::cout << "\n";
        cross_bar_.write_row(row, wd);

        cross_bar_.print();
    }

    // void write_crossbar(int row, int column, Digital_t &d) {
    //     cross_bar_.write(col, row, d);
    // }

    // void exec_sum(std::vector<bool> &mask);
    // void exec_sub(std::vector<bool> &mask1, std::vector<bool> &mask2);



    void exec_inner_product(std::vector<bool> &mask, int mul_bit = 16) {
        int round = mul_bit / rows_dac_array_.get_resolution();
        for(auto i : range(0, round)) {
            for(auto ou_col_index : range(0, cross_bar_cols_ / ou_size_cols_)) { 
                
                auto absolute_address = index_decoder_.power_on(ou_col_index);
                
                auto activation_value = pe_input_.read(absolute_address);
                
                for(auto ou_row_index : range(0, ou_size_rows_)) {
                
                    auto activation_vector = word_line_gen_.power_on(activation_value, ou_row_index);

                    auto dac_out = rows_dac_array_.power_on(activation_vector);
                    
                    auto cb_out = cross_bar_.dot(dac_out, col_ou_index);
                    
                    auto sah_out = sah_.power_on(cb_out);
                    
                    std::vector<Digital_t> adc_out;
                    for(auto col_index : range(0, ou_size_cols_)) {
                        adc_out.push_back(adc1_.power_on(sah_out[col_index]));
                        saa_[col_index] = saa_.power_on(adc_out[col_index], saa[col_index], 0);
                    }
                }
            }
            auto reg = pe_input_.read_round_n(rows_dac_array_.get_resolution(), mask);
            auto cb = cross_bar_.dot(dac);
            auto sah = sah_.power_on(cb);
            std::vector<Digital_t> adc;
            std::vector<Digital_t> saa(simd_solt_);
            for(auto i : range(0, cross_bar_cols_)) {
                if(i % 2 == 1) {
                    adc.push_back(adc1_.power_on(sah[i]));
                }
                else {
                    adc.push_back(adc2_.power_on(sah[i]));
                }
                saa[i / (mul_bit/bit_per_cell_)] = saa_.power_on(adc[i], saa[i / (mul_bit/bit_per_cell_)]);
                //std::cout << saa[i / (mul_bit/bit_per_cell_)].len_ << std::endl; 
            }
            for(auto solt : range(0, simd_solt_)) {
                auto tmp = saa_.power_on(pe_output_.read(solt), saa[solt]);
                pe_output_.write(solt, tmp);
            }
        }

    }

    // void exec_outer_product();
    // void exec_elewist_product(std::vector<bool> &mask);
};