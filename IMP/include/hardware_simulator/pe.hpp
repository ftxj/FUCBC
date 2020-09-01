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
    CrossBar cross_bar_;
    DACArray rows_dac_array_;
    DACArray cols_dac_array_;
    SimpleHold sah_;
    ShiftAdd ssa_;
    ADC adc1_;
    ADC adc2_;
    InputReg pe_input_;
    OutputReg pe_input_;
    int cross_bar_cols_;
    int cross_bar_rows_;
public:
    void exec_sum(std::vector<bool> &mask);
    void exec_sub(std::vector<bool> &mask1, std::vector<bool> &mask2);
    
    void exec_inner_product(std::vector<bool> &mask, int mul_bit = 16) {
        int round = mul_bit / rows_dac_array_.get_resolution();
        for(auto i : range(0, round)) {
            auto reg = pe_input_.read_round_n(rows_dac_array_.get_resolution(), mask);
            auto dac = rows_dac_array_.power_on(reg);
            auto cb = cross_bar_.dot(dac);
            auto sah = sah_.power_on(cb);
        }
        for(auto i : range(0, cross_bar_cols_)) {
            
        }
    }

    void exec_outer_product();
    void exec_elewist_product(std::vector<bool> &mask);
};