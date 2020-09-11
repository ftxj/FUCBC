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
    
    WordLineVectorGenerator() {}

    WordLineVectorGenerator(Config &c) : prefix_sum_(c){
        ou_rows_ = c.crossbar_ou_rows_;
    }

    DigitalBundle power_on(DigitalBundle &inp, size_t round) {
        DigitalBundle prefix_sum_res = prefix_sum_.power_on(inp);
        DigitalBundle res(inp.size(), 1);
        for(auto i : range(0, res.size())) {
            bool condition_1 = prefix_sum_res[i] >= round * ou_rows_ + 1;
            bool condition_2 = prefix_sum_res[i] < (round+1) * ou_rows_ + 1;
            res[i] =  Digital_t(condition_1 & condition_2 & inp[i], 1);
        }
        return res;
    }
};

class IndexDecoder {
private:
    PrefixSum prefix_sum_;
    size_t num_ou_horizontal_;
    size_t num_cb_rows_;

    RegFile input_index_; // ou index, ou1,ou2[1,3,4,5]; ou3,ou4[1,4,6,8]
    
    DigitalBundle prefix_sum_decode(DigitalBundle code) {
        return prefix_sum_.power_on(code);
    }
public:
    IndexDecoder() {}

    IndexDecoder(Config &c) : prefix_sum_(c), input_index_(
            c.crossbar_rows_ * (c.crossbar_cols_ / c.crossbar_ou_cols_) , 8) {
        num_ou_horizontal_ = c.crossbar_cols_ / c.crossbar_ou_cols_;
        num_cb_rows_ = c.crossbar_rows_;
    }

    void load_data(std::vector<int> &data) {
        //assert_msg(data.size() == num_cb_rows_, " ");
        DigitalBundle in_d(data.size(), 8);
        for(auto i : range(0, data.size()))
            in_d[i] = Digital_t(data[i], 8);
        input_index_.write(0, in_d);
    }

    DigitalBundle power_on(size_t ou_index) {
        return prefix_sum_decode(input_index_.read(ou_index * num_cb_rows_, num_cb_rows_));
    }
};

class PE {
public:
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

    size_t cross_bar_cols_;
    size_t cross_bar_rows_;
    size_t cross_bar_cell_bits_;
    size_t store_num_bits_;
    size_t ou_cols_;
    size_t ou_rows_;

    size_t dac_res_;
public:

    PE() : 
        cross_bar_rows_(128), cross_bar_cols_(128), cross_bar_cell_bits_(2),
        store_num_bits_(16),
        ou_cols_(8), ou_rows_(8),
        dac_res_(1)  {}

    PE(Config &c) : 
        cross_bar_(c), cols_dac_array_(c), rows_dac_array_(c), sah_(c),
        saa_(std::vector<ShiftAdd>(c.crossbar_cols_, ShiftAdd(c))), 
        adc1_(c), adc2_(c), pe_input_(c), pe_output_(c), index_decoder_(c),
        word_line_gen_(c) {
            cross_bar_cols_ = c.crossbar_cols_;
            cross_bar_rows_ = c.crossbar_rows_;
            cross_bar_cell_bits_ = c.crossbar_cell_width_;
            store_num_bits_ = c.crossbar_represent_number_bits_;
            ou_cols_ = c.crossbar_ou_cols_;
            ou_rows_ = c.crossbar_ou_rows_;
            dac_res_ = c.dac_resolution_;
        }

    Digital_t read_input_reg(int pos) {
        return pe_input_.read(pos);
    }
    
    Digital_t read_output_reg(int pos) {
        return pe_output_.read(pos);
    }

    DigitalBundle read_crossbar_dummy(int row) {
        auto row_data = cross_bar_.read_row(row);
        DigitalBundle res(cross_bar_cols_, cross_bar_cell_bits_);
        for(auto i : range(0, row_data.size())) {
            res[i] = adc1_.power_on_dummy(row_data[i]);
        }
        return res;
    }

    Digital_t read_crossbar_dummy(int row, int column) {
        auto row_data = cross_bar_.read_row(row);
        return adc1_.power_on_dummy(row_data[column]);
    }
    
    void write_input_reg(int pos, Digital_t &d) {
        pe_input_.write(pos, d);
    }

    void write_output_reg(int pos, Digital_t &d) {
        pe_input_.write(pos, d);
    }

    void write_crossbar_dummy(int row, int col, Digital_t &d) {
        cross_bar_.write_dummy(row, col, d.to_analog_dummy());
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
        cross_bar_.write_row_dummy(row,  d.to_analog_dummy());
    }

    // void write_crossbar(int row, int column, Digital_t &d) {
    //     cross_bar_.write(col, row, d);
    // }



    void tranverse_ou_rows(DigitalBundle &row_input, int ou_col_index, int input_round) {
        for(auto ou_row_index : range(0, cross_bar_rows_ / ou_rows_)) {
            auto activation_vector = word_line_gen_.power_on(row_input, ou_row_index);
            if(activation_vector.non_zero_element_num() == 0) break;
            exec_ou_compute(ou_col_index, activation_vector);
            for(auto col_index : range(ou_col_index * ou_cols_, ou_col_index * ou_cols_ + ou_cols_)) {
                auto adc_out = adc1_.power_on_dummy(sah_out[col_index]);
                saa_[col_index].power_on(adc_out, input_round * dac_res_);
            }
        }
    }

    void shift_write_to_output() {
        size_t number_of_result = (cross_bar_cols_ * cross_bar_cell_bits_) / store_num_bits_; 
        size_t number_of_cols_per_result = store_num_bits_ / cross_bar_cell_bits_;
        for(auto out_solt : range(0, number_of_result)) {
            size_t elected_ssa_index = (out_solt + 1)  * number_of_cols_per_result - 1 ;
            for(auto index : range(1,  number_of_cols_per_result)) {
                saa_[elected_ssa_index].power_on(saa_[elected_ssa_index - index].get_data(), cross_bar_cell_bits_ * index);
            }
            pe_output_.write(out_solt, saa_[elected_ssa_index].get_data());
        }
    }

    void exec_ou_compute(int ou_col_index, DigitalBundle &activation_vector) {
        assert_msg(activation_vector.non_zero_element_num() <= ou_rows_, "activation vector generate error");
        auto dac_out = rows_dac_array_.power_on_dummy(activation_vector);

        auto cb_out = cross_bar_.dot(dac_out, ou_col_index);
        // std::cout << "dump dot res:\n";
        // cb_out.dump(std::cout);
        auto sah_out = sah_.power_on_dummy(cb_out);
    }

    DigitalBundle get_active_values_from_mask(DigitalBundle &mask, DigitalBundle &addr) {
        DigitalBundle res(addrs.size(), bit_);
        Digital_t addr_pre(10000, bit_);
        for(auto i : range(0, addrs.size())) {
            if(addrs[i] != addr_pre) {
                res[i] = mask[i];
            }
            else {
                res[i] = Digital_t(0, bit_);
            }
            addr_pre = addrs[i];
        }
        return res;
    }
    

    void exec_sub(std::vector<bool> &mask1, std::vector<bool> &mask2) {
        
    }

    void exec_sum(DigitalBundle &mask) {
        for(auto ou_col_index : range(0, cross_bar_cols_ / ou_cols_)) { 
            auto absolute_address = index_decoder_.power_on(ou_col_index);
            auto active_value = get_active_values_from_mask(mask, absolute_address);
            tranverse_ou_rows(active_value, ou_col_index, 0);
        }
        shift_write_to_output();
    }

    void exec_inner_product() {
        int round = pe_input_.get_mul_bit() / rows_dac_array_.get_resolution();
        for(auto ou_col_index : range(0, cross_bar_cols_ / ou_cols_)) { 
            auto absolute_address = index_decoder_.power_on(ou_col_index);
            pe_input_.get_active_values(absolute_address);
            for(auto i : range(0, round)) {
                auto activation_value = pe_input_.read_rows_data(dac_res_, i);
                tranverse_ou_rows(activation_value, ou_col_index, i);
            }
        }
        shift_write_to_output();
    }

    // void exec_outer_product();
    // void exec_elewist_product(std::vector<bool> &mask);
};