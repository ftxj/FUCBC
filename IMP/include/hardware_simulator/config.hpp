#pragma once
#include <cmath>
#include <vector>
#include "util.hpp"

class Digital_t {
protected:
    unsigned long long data_;
public:
    int len_;
    Digital_t() : data_(0), len_(0) {}

    Digital_t(int data, int len) : data_(data), len_(len) {}
    
    Digital_t(const Digital_t &d) : data_(d.data_), len_(d.len_) {}
    
    void operator =(const Digital_t & inp) {
        data_ = inp.data_;
        len_ = inp.len_;
    }
    
    Digital_t operator +(const Digital_t & inp) {
        int len = len_ > inp.len_? len_+1 : inp.len_ + 1;
        return Digital_t(len, inp.data_ + data_);
    }

    void pack(Digital_t &inp) {
        data_ += inp.data_ << len_;
        len_ += inp.len_;
        assert_msg(len_ <= 32, "digital signal to int out of bound");
    }

    Digital_t split(int begin_, int bits_) {
        return Digital_t((data_ >> begin_) % int(std::pow(2, bits_)), bits_);
    }

    int to_int() {
        assert_msg(len_ < 32, "digital signal to int out of bound");
        return data_;
    }
    friend Digital_t operator <<(const Digital_t &inp, int bit);
    friend Digital_t operator >>(const Digital_t &inp, int bit);
};

Digital_t packer(std::vector<Digital_t> &inp) {
    Digital_t res;
    for(auto i : range(0, inp.size())) {
        res.pack(inp[i]);
    }
    assert_msg(res.len_ <= 32, "digital signal to int out of bound");
    return res;
}

Digital_t operator <<(const Digital_t &inp, int bit) {
    return Digital_t(inp.data_ << bit, inp.len_ + bit);
}

Digital_t operator >>(const Digital_t &inp, int bit) {
    return Digital_t(inp.data_ >> bit, inp.len_ + bit);
}

class Analog_t {
protected:
    double data_;
public:
    Analog_t() : data_(0) {}

    Analog_t(double d) : data_(d) {}

    Analog_t(const Analog_t &d) : data_(d.data_) {} 

    void operator =(const Analog_t & inp) {
        data_ = inp.data_;
    }

    Analog_t operator * (double frac) {
        return Analog_t(frac * data_);
    }

    Analog_t operator * (Analog_t frac) {
        return Analog_t(frac.data_ * data_);
    }

    Analog_t operator - (const Analog_t &inp) {
        return Analog_t(data_ - inp.data_);
    }

    Analog_t operator += (const Analog_t &inp) {
        return Analog_t(data_ + inp.data_);
    }
    int operator / (const Analog_t& inp) {
        return int(data_ / inp.data_);
    }
};


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
    
    size_t reram_rows_;
    size_t reram_cols_;    
    size_t reram_cell_width_; // 2, 一个 cb 存储 4KB 数据，一个 cb 相当于一个 128 * 8（128*2 / 32）的 SIMD 指令
    
    size_t adc_resolution_;// 5, each cross bar has 2 adc
    size_t dac_resolution_; // 2, each cross bar has 2 dac
    size_t input_reg_per_cluster_;// 256 byte = 32 * 8  
    size_t output_reg_per_cluster;// 256 byte = 32 * 8
};