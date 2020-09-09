#pragma once
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include "hardware_simulator/config.hpp"
#include "hardware_simulator/isa.hpp"
#include "util.hpp"

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
    /*
        11   0.9    00          01          10          11

                    0           0.9         1.8         2.7
                    -10e-10                             0.0078
                    0           3           6           9
        0.6
        0.3
        0.0
    */
    DAC() : latency_(1), resolution_(2), digital_max_(Digital_t(3, 2)), analog_max_(Analog_t(0.9)) {}
    DAC(Config) {}
    
    int get_latency() { return latency_; }
    int get_resolution() { return resolution_; }

    Analog_t power_on(Digital_t &inp) {
        num_access_ += 1;
        assert_msg(inp.len_ == resolution_, inp.len_ << "!=" << resolution_ << "," << inp.to_int() << ", dac input error!!");
        return digital_to_analog(inp);
    }
    // /static int component_number_;
};

class DACArray {
    int size_;
    int latency_;
    int resolution_;
    std::vector<DAC> dac_array_;
public:
    DACArray() : resolution_(2), size_(128), latency_(1), dac_array_(128, DAC()) {}
    DACArray(Config);
    
    int get_latency() { return latency_; }
    int get_resolution() { return resolution_; }

    std::vector<Analog_t> power_on(std::vector<Digital_t> &inp) {
        std::vector<Analog_t> out;
        assert_msg(inp.size() == size_, "out of bound" + std::to_string(inp.size()));
        for(auto i : range(0, size_)) {
            out.push_back(dac_array_[i].power_on(inp[i]));
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
        Analog_t diff = inp - input_min_;
        Analog_t max_diff = input_max_ - input_min_;
        int stage = (diff * double(std::pow(2, resolution_))) / max_diff;
        return Digital_t(stage, resolution_);
    }
public:
    ADC() : latency_(50), resolution_(5),  input_min_(0), input_max_(346) {} // TODO
    ADC(Config);

    int get_latency() { return latency_; }

    Digital_t power_on(Analog_t &inp) {
        return analog_to_digital(inp);
    }
};

class SimpleHold {
    int num_access_;
    int latency_;
    std::vector<Analog_t> value_;
public:
    SimpleHold() : latency_(1), value_(128, Analog_t()) {} // TODO
    SimpleHold(Config);

    int get_latency() { return latency_; }

    std::vector<Analog_t> power_on(std::vector<Analog_t> &inp) {
        assert_msg(inp.size() == value_.size(), "simple & hold length error!!");
        num_access_ += 1;
        value_ = inp;
        return value_;
    }
    
    Analog_t get_latch(int index) { return value_[index]; }
};

class ShiftAdd {
    int num_access_;
    int latency_;
    int bit_width_;
    int shift_bit_;
public:
    ShiftAdd() : latency_(1), shift_bit_(2), bit_width_(64) {} // TODO
    ShiftAdd(Config);

    int get_latency() { return latency_; }

    Digital_t power_on(Digital_t &inp1, Digital_t &inp2, size_t shift_bit_) {
        num_access_++;
        //std::cout << inp1.len_ << "," << inp2.len_ << std::endl;
        assert_msg(inp1.len_ <= bit_width_, "shift&add outof bound" << inp1.len_);
        assert_msg(inp2.len_ <= bit_width_, "shift&add outof bound" << inp2.len_);
        return Digital_t(inp1 + (inp2 << shift_bit_));
    }
};

class Reg {

};

class InputReg {
    std::vector<Digital_t> regs_;
    int bit_;
    int round_;
    int num_access_;
public:
    InputReg() : regs_(128), bit_(32) {}
    InputReg(int rows, int bit) : regs_(rows), bit_(bit) {
        assert_msg(bit <= 32, "just support 32 bits");
    }
    std::vector<Digital_t> read_round_n(int bits, std::vector<bool> &mask) {
        num_access_++;
        std::vector<Digital_t> res(regs_.size());
        assert_msg(mask.size() == regs_.size(), "mask error");
        for(auto i : range(0, res.size())) {
            res[i] = mask[i]? regs_[i].split(round_ * bits, bits) : Digital_t(0, bits);
        }
        round_ = (round_ + 1) % (bit_ / bits);
        return res;
    }
    Digital_t &read(int pos) {
        return regs_[pos];
    }
    void write(int pos,  Digital_t &d) {
        regs_[pos] = d;
    }
    void print() {
        for(auto d : regs_) {
            d.print();
            std::cout << "\n";
        }
    }
};



class OutputReg {
    std::vector<Digital_t> regs_;
    int bit_;
    int num_access_;
public:
    OutputReg() : regs_(8), bit_(32) {}
    OutputReg(int solt, int bit) : regs_(solt), bit_(bit) {
        assert_msg(bit <= 32, "just support 32 bits");
    }
    void write(std::vector<Digital_t> &data) {
        assert_msg(data.size() == regs_.size(), "write width isn't same!!!");
        assert_msg(data[0].len_ <= bit_, "write width isn't g!!!");
        for(auto i : range(0, regs_.size())) {
            regs_[i] = data[i];
        }
    }

    void write(int solt, Digital_t &d) {
        regs_[solt] = d;
    }

    Digital_t& read(int solt) {
        return regs_[solt];
    }
};

class RegFile {
    std::vector<Digital_t> regs_;
    int bit_;
    int num_access_;
public:
    RegFile() : regs_(512), bit_(32) {}
    RegFile(int size, int bit) : regs_(size), bit_(bit) {
        assert_msg(bit <= 32, "just support 32 bits");
    }

    void write(int addr, Digital_t d) {
        assert_msg(d.len_ <= bit_, "reg file write outof bound");
        assert_msg(addr <= regs_.size(), "reg file write outof bound");
        regs_[addr] = d;
    }
    
    Digital_t read(int addr) {
        assert_msg(addr <= regs_.size(), "reg file write outof bound");
        return regs_[addr];
    }
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

class SharedBus {

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
    
    Instruction get(int pc) {
        assert_msg(pc <= size_, "Instr buf outof bound");
        return buf_[pc];
    }

    void add(Instruction &inst) {
        buf_[inc] = inst;
        inc++;
    }

    int get_latency() { return latency_; }
    int inc;
};


class PrefixSum {
public:
    std::vector<Digital_t> power_on(std::vector<Digital_t> &inp) {
        std::vector<Digital_t> res(inp.size());
        Digital_t tmp;
        for(auto x : range(0, inp.size())) {
            res[x] = tmp + inp[x];
            tmp = res[x];
        }
        return res;
    }
};

class CMP {
public:
    std::vector<Digital_t> power_on(std::vector<Digital_t> &inp) {
        std::vector<Digital_t> res(inp.size());
        Digital_t tmp;
        for(auto x : range(0, inp.size())) {
            res[x] = tmp + inp[x];
            tmp = res[x];
        }
        return res;
    }
};

class Mask {

};