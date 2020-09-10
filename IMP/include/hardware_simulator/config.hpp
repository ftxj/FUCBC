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
    void operator =(const bool & inp) {
        data_ = inp?1:0;
        len_ = 1;
    }


    Digital_t operator +(const Digital_t & inp) {
        int len = len_ > inp.len_? len_ : inp.len_;
        return Digital_t(inp.data_ + data_, len);
    }

    

        
    bool operator >=(const Digital_t & inp) {
        return data_ >= inp.data_;
    }
    bool operator >=(const int & inp) {
        return data_ >= inp;
    }
    bool operator <=(const Digital_t & inp) {
        return data_ <= inp.data_;
    }
    bool operator <=(const int & inp) {
        return data_ <= inp;
    }
    bool operator >(const Digital_t & inp) {
        return data_ > inp.data_;
    }
    bool operator <(const Digital_t & inp) {
        return data_ < inp.data_;
    }
    bool operator >(const int & inp) {
        return data_ > inp;
    }
    bool operator <(const int & inp) {
        return data_ < inp;
    }

    bool operator &(const bool & inp) {
        return data_ & inp;
    }

    void pack(Digital_t &inp) {
        data_ += inp.data_ << len_;
        len_ += inp.len_;
        assert_msg(len_ <= 64, "digital signal to int out of bound");
    }

    Digital_t split(int begin_, int bits_) {
        return Digital_t((data_ >> begin_) % int(std::pow(2, bits_)), bits_);
    }

    int to_int() {
        return data_;
    }

    void print() {
        std::cout << data_ << "(" << len_ << ")";
    }
    friend Digital_t operator <<(const Digital_t &inp, int bit);
    friend Digital_t operator >>(const Digital_t &inp, int bit);
    friend bool operator &(bool inp1, const Digital_t &inp2);
};

class DigitalBundle {
    std::vector<Digital_t> data_;
public:
    size_t signal_len_;

    DigitalBundle(size_t size);
    
    Digital_t& operator [](int index) {
        return data_[index];
    }
    size_t size();

    std::vector<Digital_t>::iterator begin() {
        return data_.begin();
    }
    std::vector<Digital_t>::iterator end() {
        return data_.end();
    }
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

bool operator &(bool inp1, const Digital_t &inp2) {
    return inp2.data_ & inp1;
}

class Analog_t {
protected:
    float data_;
    const float epsilon = 1e-6;
public:
    Analog_t() : data_(0) {}

    Analog_t(float d) : data_(d) {}

    Analog_t(const Analog_t &d) : data_(d.data_) {} 

    void print() {
        std::cout << data_;
    }

    float to_float() {
        return data_;
    }

    void operator =(const Analog_t & inp) {
        data_ = inp.data_;
    }

    void operator =(const float & inp) {
        data_ = inp;
    }

    Analog_t operator + (const Analog_t &inp) const {
        return Analog_t(inp.data_ + data_);
    }

    Analog_t operator * (float frac) {
        return Analog_t(frac * data_);
    }

    Analog_t operator * (const Analog_t &inp) {
        return Analog_t(inp.data_ * data_);
    }

    Analog_t operator - (const Analog_t &inp) {
        return Analog_t(data_ - inp.data_);
    }

    Analog_t operator += (const Analog_t &inp) {
        data_ += inp.data_;
        return *this;
    }
    int operator / (const Analog_t& inp) {
        return int(data_ / inp.data_);
    }

    bool operator ==(const Analog_t& inp) const {
        return std::abs(data_ - inp.data_) < epsilon;
    }
};


class AnalogBundle {
    std::vector<Analog_t> data_;
    size_t size_;
public:
    AnalogBundle(size_t size) : data_(size), size_(size) {}
    AnalogBundle(size_t size, Analog_t val) : data_(size, val), size_(size) {}
    AnalogBundle(const std::vector<Analog_t> &d) : data_(d), size_(d.size()) {}
    AnalogBundle(const std::vector<float> &d) : data_(d.size()), size_(d.size()) {
        for(auto dx : range(0, size_)) {
            data_[dx] = d[dx];
        }
    }


    void operator =(const AnalogBundle & inp) {
        data_ = inp.data_;
    }

    void operator =(const std::vector<float> & inp) {
        for(auto dx : range(0, size_)) {
            data_[dx] = inp[dx];
        }
    }


    Analog_t& operator [](int index) {
        return data_[index];
    }
    size_t size() { return size_; }

    std::vector<Analog_t>::iterator begin() {
        return data_.begin();
    }
    std::vector<Analog_t>::iterator end() {
        return data_.end();
    }
    void dump(std::ostream &out) {
        for(auto x : range(0, size_)) {
            out << "{" << data_[x].to_float() << "},";
            if(x % 10 == 0) out << "\n";
        }
    }

    bool operator ==(const AnalogBundle& inp) const {
        return data_ == inp.data_;
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