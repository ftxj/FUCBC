#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>
#include "error/assert.hpp"
#include "hardware_simulator/digital.hpp"
#include "hardware_simulator/crossbar.hpp"
#include "util.hpp"

class PE {
private:
    CrossBar cross_bar_;
    DACArray rows_dac_array_;
    DACArray cols_dac_array_;
    SimpleHold sah_;
    ShiftAdd ssa_;
    ADC adc1_;
    ADC adc2_;
    InputReg pe_input_;
    OutputReg pe_output_;
    
    int cross_bar_cols_;
    int cross_bar_rows_;
    int bit_per_cell_;
    int simd_solt_;

    bool locked_;
public:
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
    }
    void write_output(int pos, Digital_t &d) {
        pe_input_.write(pos, d);
    }
    void write_crossbar(int row, std::vector<Digital_t> &d) {
        ftxj_debug_print("real write 2");
        
        auto wd = cols_dac_array_.power_on(d);
        ftxj_debug_print("real write 2");
        cross_bar_.write_row(row, wd);
    }

    // void write_crossbar(int row, int column, Digital_t &d) {
    //     cross_bar_.write(col, row, d);
    // }

    // void exec_sum(std::vector<bool> &mask);
    // void exec_sub(std::vector<bool> &mask1, std::vector<bool> &mask2);

    void exec_inner_product(std::vector<bool> &mask, int mul_bit = 16) {
        assert_msg(!locked_, "pe locked, but run dot");
        int round = mul_bit / rows_dac_array_.get_resolution();
        for(auto i : range(0, round)) {
            auto reg = pe_input_.read_round_n(rows_dac_array_.get_resolution(), mask);
            auto dac = rows_dac_array_.power_on(reg);
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
                saa[i / (mul_bit/bit_per_cell_)] = ssa_.power_on(adc[i], saa[i / (mul_bit/bit_per_cell_)]);
            }
            for(auto solt : range(0, simd_solt_)) {
                auto tmp = ssa_.power_on(pe_output_.read(solt), saa[solt]);
                pe_output_.write(solt, tmp);
            }
        }
    }

    // void exec_outer_product();
    // void exec_elewist_product(std::vector<bool> &mask);
};