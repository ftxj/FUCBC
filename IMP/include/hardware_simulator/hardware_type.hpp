#pragma once
#include <cmath>
#include <vector>
#include "util.hpp"

class Analog_t;
class AnalogBundle;

class Digital_t {
protected:
    unsigned long long data_;
public:
    int len_;
    Digital_t() : data_(0), len_(0) {}

    Digital_t(int data, int len) : data_(data), len_(len) {}
    
    Digital_t(const Digital_t &d) : data_(d.data_), len_(d.len_) {}
    
    bool is_zero() {
        return data_ == 0;
    }
    
    void operator =(const Digital_t & inp) {
        data_ = inp.data_;
        len_ = inp.len_;
    }

    // void operator =(const bool & inp) {
    //     data_ = inp?1:0;
    //     len_ = 1;
    // }

    Digital_t operator +(const Digital_t & inp) {
        int len = len_ > inp.len_? len_ : inp.len_;
        if(inp.data_ + data_ >= std::pow(2, len)) {
            len ++;
        }
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
    bool operator !=(const Digital_t & inp) {
        return data_ != inp.data_;
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
        return Digital_t((data_ >> begin_) & (int(std::pow(2, bits_) - 1)), bits_);
    }

    int to_int() {
        return data_;
    }

    void print() {
        std::cout << data_ << "(" << len_ << ")";
    }

    Analog_t to_analog_dummy();
    
    friend Digital_t operator <<(const Digital_t &inp, int bit);
    friend Digital_t operator >>(const Digital_t &inp, int bit);
    friend bool operator &(bool inp1, const Digital_t &inp2);
};

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

    bool is_zero() {
        return std::abs(data_) < epsilon;
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
    
    void dump(std::ostream &out) {
        bool out_empty = false;
        int out_empty_begin = 0;
        for(auto d : range(0, size_)) {
            if(!data_[d].is_zero()) {
                if(out_empty) {
                    out << d  << "}, "; 
                }
                out << "{" << data_[d].to_float() << "}, ";
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
             out << size_ << "}"; 
        }
        out << "\n";
    }

    void operator =(const AnalogBundle & inp) {
        data_ = inp.data_;
    }

    void operator =(const std::vector<float> & inp) {
        for(auto dx : range(0, size_)) {
            data_[dx] = inp[dx];
        }
    }

    bool is_zero() {
        for(auto d : range(0, size_)) {
            if(!data_[d].is_zero()) return false;
        }
        return true;
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

    bool operator ==(const AnalogBundle& inp) const {
        return data_ == inp.data_;
    }
};


class DigitalBundle {
    std::vector<Digital_t> data_;
public:
    size_t signal_len_;

    DigitalBundle() : data_(0), signal_len_(0) {}
    DigitalBundle(size_t size, size_t len) : data_(size), signal_len_(len) {}

    DigitalBundle(const std::vector<Digital_t> &v)  {
        data_ = v;
    }
    
    std::vector<int> to_int_vector() {
        std::vector<int> res;
        for(auto i : range(0, data_.size())) {
            res.push_back(data_[i].to_int());
        }
        return res;
    }

    Digital_t& operator [](int index) {
        return data_[index];
    }
    
    size_t size() {
        return data_.size();
    }
    
    AnalogBundle to_analog_dummy() {
        AnalogBundle res(data_.size());
        for(auto i : range(0, res.size())) {
            res[i] = static_cast<float>(data_[i].to_int());
        }
    }

    std::vector<Digital_t>::iterator begin() {
        return data_.begin();
    }
    std::vector<Digital_t>::iterator end() {
        return data_.end();
    }

    DigitalBundle split(int begin, int len) {
        return DigitalBundle(std::vector<Digital_t>(data_.begin() + begin, data_.begin() + begin + len));
    }

    size_t non_zero_element_num() {
        size_t res = 0;
        for(auto x : data_) {
            if(!x.is_zero()) {
                res++;
            }
        }
        return res;
    }

    void dump(std::ostream &out) {
        bool out_empty = false;
        int out_empty_begin = 0;
        for(auto d : range(0, data_.size())) {
            if(!data_[d].is_zero()) {
                if(out_empty) {
                    out << d  << "}, "; 
                }
                out << "{" << data_[d].to_int() << "}, ";
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
             out << data_.size() << "}"; 
        }
        out << "\n";
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
