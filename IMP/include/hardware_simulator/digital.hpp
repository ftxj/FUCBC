#pragma once
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include "hardware_simulator/config.hpp"
#include "hardware_simulator/isa.hpp"

class DAC {
    int num_access_;
    int latency_;
    int resolution_;

    Digital_t digital_max_;
    Analog_t analog_max_;

    Analog_t digital_to_analog(Digital_t &inp) {
        double frac = inp.to_int() / double(digital_max_.to_int());
        return analog_max_ * frac;
    }
public:
    DAC() : latency_(1), resolution_(2) { DAC::component_number_++; }
    DAC(Config);
    
    int get_latency() { return latency_; }
    int get_resolution() { return resolution_; }

    Analog_t power_on(Digital_t &inp) {
        num_access_ += 1;
        return digital_to_analog(inp);
    }
    static int component_number_;
};

class DACArray {
    int size_;
    int latency_;
    int resolution_;
    std::vector<DAC> dac_array_;
public:
    DACArray() : size_(128), latency_(1), dac_array_(128, DAC()) {}
    DACArray(Config);
    
    int get_latency() { return latency_; }
    int get_resolution() { return resolution_; }

    std::vector<Analog_t> power_on(std::vector<Digital_t> &inp) {
        std::vector<Analog_t> out;
        for(auto i : range(0, size_)) {
            out.push_back(digital_to_analog(inp));
        }
        return out;
    }
};

class ADC {
    int num_access_;
    int latency_;
    int resolution_;

    Analog_t input_min_;
    Analog_t input_max_;
    Analog_t step_degree_;
    // TODO
    Digital_t analog_to_digital(Analog_t &inp) {
        return Digital_t((inp - input_min_) / step_degree_);
    }
public:
    ADC() : latency_(50), resolution_(5) {} // TODO
    ADC(Config);

    int get_latency() { return latency_; }

    Digital_t power_on(Analog_t &inp) {
        return analog_to_digital(inp);
    }
};

class SimpleHold {
    int num_access_;
    int latency_;
    int bit_width_;
    std::vector<Digital_t> value_;
public:
    SimpleHold() : bit_width_(2), latency_(1), value_(2, Digital_t()) {} // TODO
    SimpleHold(Config);

    int get_latency() { return latency_; }

    std::vector<Digital_t> power_on(std::vector<Digital_t> &inp) {
        num_access_ += 1;
        value_ = inp;
        return value;
    }
    Digital_t get_latch(int index) { return value_[index]; }
};

class ShiftAdd {
    int num_access_;
    int latency_;
    int bit_width_;
    int shift_bit_;
public:
    ShiftAdd() : bit_width_(2), latency_(1), shift_bit_(2) {} // TODO
    ShiftAdd(Config);

    int get_latency() { return latency_; }

    Digital_t power_on(Digital_t &inp1, Digital_t &inp2) {
        assert_msg(inp1.len_ > bit_width_, "shift&add outof bound");
        assert_msg(inp2.len_ > bit_width_, "shift&add outof bound");
        return inp1 + (inp2 << shift_bit_);
    }
};

class Reg {

};

class InputReg {

public:
    std::vector<Digital_t> read_round_n(int bits, std::vector<bool> &mask);
};

class RegFile {

};

class LUT {
    int num_access_;
    int latency_;
    int search_bit_width_;
    int result_bit_width_;
    int size_;
    std::vector<Digital_t> data_;
public:
    LUT() : search_bit_width_(8), result_bit_width_(8), size_(512), data_(512, Digital_t()) {} // TODO
    LUT(Config);

    int get_latency() { return latency_; }

    Digital_t power_on(Digital_t &inp) {
        assert_msg(inp.len_ > search_bit_width_, "LUT outof bound");
        assert_msg(inp.to_int() > size_, "LUT outof bound");
        num_access_ += 1;
        return data_[inp.to_int()];
    }
};

class NoC {
    int num_access_;
    int num_cycles_;
    int latency_;

    int start_cycle_;
public:
    NoC() : latency_(1) {} // TODO
    NoC(Config);

    int get_latency() { return latency_; }

    void start(int cycle) { start_cycle_ = cycle; }
    void stop(int cycle) { num_cycles_ += cycle - start_cycle_; start_cycle_ = 0; }
    
    void power_on(GlobalAddress, GlobalAddress);
};

class HTree {

};

class InstrBuffer {
    std::vector<Instruction> buf_;
    int num_access_;
    int size_;
    int latency_;
public:
    InstrBuffer() : latency_(1), size_(512),  buf_(512, Instruction()) {} // TODO
    InstrBuffer(Config);
    
    void load();

    void write();

    int get_latency() { return latency_; }
};

