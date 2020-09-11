#pragma once
#include <cmath>
#include <vector>
#include "util.hpp"
#include "hardware_simulator/hardware_type.hpp"

class Config {
    int adc_resolution_needed(int rows, int input_per_cycle, int cell_bit) {
        // from ISAAC
        if(input_per_cycle > 1 && cell_bit > 1) {
            return input_per_cycle + cell_bit + std::log2(rows);
        }
        return input_per_cycle + cell_bit + std::log2(rows) - 1;
    }
public:
    size_t num_tiles_;
    size_t num_cores_;
    size_t num_pe_;
    
    size_t crossbar_rows_;
    size_t crossbar_cols_;    
    size_t crossbar_cell_width_; // 2, 一个 cb 存储 4KB 数据，一个 cb 相当于一个 128 * 8（128*2 / 32）的 SIMD 指令
    size_t crossbar_ou_rows_;
    size_t crossbar_ou_cols_;
    size_t crossbar_represent_number_bits_;

    size_t dac_resolution_;
    size_t dac_latency_;
    Digital_t dac_digital_max_;
    Analog_t dac_analog_max_;

    size_t adc_resolution_;
    size_t adc_latency_;
    Analog_t adc_input_min_;
    Analog_t adc_input_max_;

    size_t simple_hold_latency_;
    size_t shift_add_latency_;
    size_t shift_add_width_;

    size_t input_reg_size_;
    size_t input_reg_data_width_;

    size_t output_reg_size_;
    size_t output_reg_data_width_;

    size_t perfix_sum_latency_;
    
};