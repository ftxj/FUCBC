#pragma once
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include "hardware_simulator/config.hpp"
#include "hardware_simulator/isa.hpp"
#include "hardware_simulator/hardware_type.hpp"
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
    DAC() : latency_(1), resolution_(2), digital_max_(Digital_t(3, 2)), analog_max_(Analog_t(0.9)) {}
    
    DAC(Config &c) {
        latency_ = c.dac_latency_;
        resolution_ = c.dac_resolution_;
        digital_max_ = c.dac_digital_max_;
        analog_max_ = c.dac_analog_max_;
    }
    
    int get_latency() { return latency_; }
    int get_resolution() { return resolution_; }

    Analog_t power_on(Digital_t &inp) {
        num_access_ += 1;
        assert_msg(inp.len_ == resolution_, inp.len_ << "!=" << resolution_ << "," << inp.to_int() << ", dac input error!!");
        return digital_to_analog(inp);
    }

    Analog_t power_on_dummy(Digital_t &inp) {
        num_access_ += 1;
        assert_msg(inp.len_ == resolution_, inp.len_ << "!=" << resolution_ << "," << inp.to_int() << ", dac input error!!");
        return Analog_t(static_cast<float>(inp.to_int()));
    }
};


class DACArray {
    int size_;
    int latency_;
    int resolution_;
    std::vector<DAC> dac_array_;
public:
    DACArray() : resolution_(2), size_(128), latency_(1), dac_array_(128, DAC()) {}
    DACArray(Config &c) : dac_array_(c.crossbar_rows_, DAC(c)){
        size_ = c.crossbar_rows_;
        resolution_ = c.dac_resolution_;
        latency_ = c.dac_latency_;
    }
    
    int get_latency() { return latency_; }
    int get_resolution() { return resolution_; }

    AnalogBundle power_on(DigitalBundle &inp) {
        AnalogBundle out(inp.size());
        assert_msg(inp.size() == size_, "out of bound" + std::to_string(inp.size()));
        for(auto i : range(0, size_)) {
            out[i]  = dac_array_[i].power_on(inp[i]);
        }
        return out;
    }

    AnalogBundle power_on_dummy(DigitalBundle &inp) {
        AnalogBundle out(inp.size());
        assert_msg(inp.size() == size_, "out of bound" + std::to_string(inp.size()));
        for(auto i : range(0, size_)) {
            out[i]  = dac_array_[i].power_on_dummy(inp[i]);
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
    //Analog_t step_degree_;
    // TODO
    Digital_t analog_to_digital(Analog_t &inp) {
        Analog_t diff = inp - input_min_;
        Analog_t max_diff = input_max_ - input_min_;
        int stage = (diff * double(std::pow(2, resolution_))) / max_diff;
        return Digital_t(stage, resolution_);
    }
public:
    ADC() : latency_(50), resolution_(5),  input_min_(0), input_max_(346) {} // TODO
    ADC(Config &c) {
        latency_ = c.adc_latency_;
        resolution_ = c.adc_resolution_;
        input_min_ = c.adc_input_min_;
        input_max_ = c.adc_input_max_;
    }

    int get_latency() { return latency_; }

    Digital_t power_on(Analog_t &inp) {
        return analog_to_digital(inp);
    }

    Digital_t power_on_dummy(Analog_t &inp) {
        return Digital_t(int(inp.to_float()), resolution_);
    }
};

class SimpleHold {
    int num_access_;
    int latency_;
    AnalogBundle value_;
public:
    SimpleHold() : latency_(1), value_(128) {} // TODO
    SimpleHold(Config &c) : value_(c.crossbar_cols_) {
        latency_ = c.simple_hold_latency_;
    }

    int get_latency() { return latency_; }

    AnalogBundle power_on(AnalogBundle &inp) {
        assert_msg(inp.size() == value_.size(), "simple & hold length error!!");
        num_access_ += 1;
        value_ = inp;
        return value_;
    }
    
    AnalogBundle power_on_dummy(AnalogBundle &inp) {
        assert_msg(inp.size() == value_.size(), "simple & hold length error!!");
        return inp;
    }

    Analog_t get_latch(int index) { return value_[index]; }
};


class ShiftAdd {
    int num_access_;
    int latency_;

    size_t add_width_;
    Digital_t reg_;
public:
    ShiftAdd() : latency_(1), add_width_(64) {} // TODO
    ShiftAdd(Config &c) {
        latency_ = c.shift_add_latency_;
        add_width_ = c.shift_add_width_;
    }

    int get_latency() { return latency_; }

    Digital_t power_on(Digital_t &inp1, size_t shift_bit_) {
        num_access_++;
        reg_ = reg_ + (inp1 << shift_bit_);
        assert_msg(reg_.len_ <= add_width_, 
            "shift&add need result <=" << add_width_ <<
            ", but given " << reg_.len_);
        if(reg_.len_ > 32) reg_.len_ = 32;
        return reg_;
    }

    Digital_t power_on(Digital_t &inp1, Digital_t &inp2, size_t shift_bit_) {
        num_access_++;
        reg_ = inp1_ + (inp2 << shift_bit_);
        assert_msg(reg_.len_ <= add_width_, 
            "shift&add need result <=" << add_width_ <<
            ", but given " << reg_.len_);
        if(reg_.len_ > 32) reg_.len_ = 32;
        return reg_;
    }

    Digital_t& get_data() {
        return reg_;
    }
};


class RegFile {
protected:
    DigitalBundle regs_;
    int num_access_;
    int bit_;
public:
    RegFile() : regs_(512, 32), bit_(32) {}
    
    RegFile(int size, int bit) : regs_(size, bit), bit_(bit) {
        assert_msg(bit <= 32, "just support 32 bits");
    }

    void write(int addr, Digital_t &d) {
        assert_msg(d.len_ <= bit_, "reg file write outof bound");
        assert_msg(addr <= regs_.size(), "reg file write outof bound");
        regs_[addr] = d;
    }

    void write(int addr, DigitalBundle &d) {
        assert_msg(d.signal_len_ <= bit_, "reg file write outof bound");
        assert_msg(addr + d.size() <= regs_.size(), "reg file write outof bound");
        for(auto iter : range(addr, addr + d.size())) {
            regs_[iter] = d[iter - addr];
        }
    }

    void load_data(std::vector<int> &data, int len) {
        //assert_msg(data.size() == num_cb_rows_, " ");
        DigitalBundle in_d(data.size(), len);
        for(auto i : range(0, data.size()))
            in_d[i] = Digital_t(data[i], len);
        write(0, in_d);
    }
    
    DigitalBundle read(int addr, int len) {
        assert_msg(addr + len <= regs_.size(), "reg file write outof bound");
        return regs_.split(addr, len);
    }

    Digital_t read(int addr) {
        assert_msg(addr <= regs_.size(), "reg file write outof bound");
        return regs_[addr];
    }

    void dump(std::ostream &out) {
        for(auto index : range(0, regs_.size())) {
            if(regs_[index].to_int() != 0) {
                out << "[" << index << "," << regs_[index].to_int() << "]\n";
            }
        }
    }

};

class InputReg : public RegFile {
    // TODO, input data orginazation
    DigitalBundle active_values_;
    size_t ou_rows_;
    DigitalBundle read_round_n(int bits, int round, DigitalBundle &inp) {
        DigitalBundle res(inp.size(), bit_);
        for(auto i : range(0, res.size())) {
            res[i] = inp[i].split(round * bits, bits);
        }
        return res;
    }
public:
    InputReg() : RegFile() {}
    InputReg(int rows, int bit) : RegFile(rows, bit) {}
    InputReg(Config &c) : RegFile(c.input_reg_size_, c.input_reg_data_width_) , 
        active_values_(c.crossbar_ou_rows_, c.input_reg_data_width_) {}

    DigitalBundle read_rows_data(int bits, int round) {
        num_access_++;
        //DigitalBundle ou_rows_data = regs_.split(ou_col_index * ou_rows_, ou_rows_);
        DigitalBundle res = read_round_n(bits, round, active_values_);
        return res;
    }

    void get_active_values(DigitalBundle &addrs) {
        DigitalBundle res(addrs.size(), bit_);
        Digital_t addr_pre(10000, bit_);
        for(auto i : range(0, addrs.size())) {
            if(addrs[i] != addr_pre) {
                res[i] = RegFile::read(addrs[i].to_int());
            }
            else {
                res[i] = Digital_t(0, bit_);
            }
            addr_pre = addrs[i];
        }
        active_values_ = res;
    }

    size_t get_mul_bit() {
        return bit_;
    }
};


class OutputReg : public RegFile {
public:
    OutputReg() : RegFile() {}
    OutputReg(int solt, int bit) : RegFile(solt, bit) {}
    OutputReg(Config &c) :  RegFile(c.output_reg_size_, c.output_reg_data_width_) {}
};


class PrefixSum {
    size_t num_access_;
    size_t latency_;
    size_t width_;
public:
    PrefixSum() : latency_(1), width_(128) {}
    PrefixSum(Config &c) {
        latency_ = c.perfix_sum_latency_;
        width_ = c.crossbar_rows_;
    }

    DigitalBundle power_on(DigitalBundle &inp) {
        assert_msg(inp.size() == width_, 
            "PerfixSum must given " << width_ << 
            "values, but given " <<
            inp.size());
        DigitalBundle res(inp.size(), width_);
        Digital_t tmp;
        for(auto x : range(0, inp.size())) {
            res[x] = tmp + inp[x];
            tmp = res[x];
        }
        return res;
    }

    DigitalBundle power_on_dummy(DigitalBundle &inp) {
        return power_on(inp);
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