#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>
#include "hardware_simulator/pe.hpp"
#include "hardware_simulator/isa.hpp"
#include "hardware_simulator/config.hpp"
#include "hardware_simulator/digital.hpp"
#include "error/assert.hpp"

class Core {
    // SIMD exec model
    std::vector<PE> pes_;
    InstrBuffer inst_buf_;
    RegFile reg_;
    LUT lut_;
    SharedBus bus_;
    

    class Lock {
        std::vector<int> locked_;
    public:
        void lock(int id, int clock, int write_crossbar_cycles) { locked_[id] = clock + write_crossbar_cycles; }
        void is_lock(int id, int clock) { return locked_[id] <= clock; }
    };
    Lock locks;

    
    size_t row_size_;
    size_t col_size_;
    int write_crossbar_cycles;
    int input_buf_size;
    int output_buf_size;
    int reg_buf_size;

    int pc;
    int STATE;
    enum {
        RUNNING,
        WAIT,
        INIT
    };
    int begin_clock_;
    
    std::vector<Digital_t> read_data_from_reg(LocalAddress &src) {
        std::vector<Digital_t> res;
        int addr = src.address;
        if(addr < input_buf_size * pes_size()) {
            res = pes_[addr / input_buf_size].read_input(addr % input_buf_size);
        }
        else if(addr < input_buf_size * pes_size() + output_buf_size * pes_size()) {
            addr = addr - input_buf_size * pes_size();
            res = pes_[addr / output_buf_size].read_output(addr % output_buf_size);
        }
        else {
            addr = addr - input_buf_size * pes_size() - output_buf_size * pes_size();
            res = reg_.read(addr);
        }
    }
    
    std::vector<Digital_t> read_data_from_crossbar(LocalAddress &src) {
        int addr = src.address;
        return pes_[addr / row_size_].read_crossbar(addr % row_size_);
    }

    void write_data_to_reg(LocalAddress &dst, std::vector<Digital_t> data) {
        int addr = src.address;
        if(addr < input_buf_size * pes_size()) {
            pes_[addr / input_buf_size].write_input(addr % input_buf_size, data);
        }
        else if(addr < input_buf_size * pes_size() + output_buf_size * pes_size()) {
            addr = addr - input_buf_size * pes_size();
            pes_[addr / output_buf_size].write_input(addr % output_buf_size, data);
        }
        else {
            addr = addr - input_buf_size * pes_size() - output_buf_size * pes_size();
            reg_.write(addr, data);
        }
    }

    void write_data_to_crossbar(LocalAddress &dst, std::vector<Digital_t> data, int clock) {
        int addr = src.address;
        assert_msg(!locks.is_lock(addr / row_size_), clock, "locked!!");
        locks.lock(addr / row_size_, clock, write_crossbar_cycles);
        pes_[addr / row_size_].write_crossbar(addr % row_size_, data);
    }

    std::vector<Digital_t> read_data(LocalAddress &src) {
        if(src.on_reg) return read_data_from_reg(src);
        if(src.on_cb) return read_data_from_crossbar(src);
        assert_msg(0, "local address outof bound");
        return std::vector<Digital_t>();
    }

    std::vector<Digital_t> write_data(LocalAddress &dst, std::vector<Digital_t> &data, int clock) {
        if(src.on_reg) return write_data_to_reg(dst, data);
        if(src.on_cb) return write_data_to_crossbar(dst, data, clock);
        assert_msg(0, "local address outof bound");
    }


    void run_able(std::vector<bool> &pe_mask, int clock) {
        for(auto i : range(0, pe_mask.size())) {
            assert_msg(!locks.is_lock(i, clock), "locked!! check your code");
        }
    }
public:

    Core() : row_size_(128), col_size_(128), write_crossbar_cycles(1000), 
            input_buf_size(4), output_buf_size(4), reg_buf_size(4) {}

    Core(Config &config) {}

    void load_instruction();

    void exec_dot(std::vector<bool> &mask1, std::vector<bool> &mask2) {
        for(auto i : range(0, pes_.size())) {
            if(mask1[i]) pes_[i].exec_inner_product(mask2);
        }
    }
    
    void exec_mov(LocalAddress &src, LocalAddress &dst) {
        std::vector<Digital_t> data = read_data(src);
        write_data(dst, src);
    }

    //void exec_movi(LocalAddress &dst, int imm);
    //void exec_movs(LocalAddress &src, LocalAddress &dst, std::vector<bool> &mask);

    //void exec_load(TileAddress &src, LocalAddress &dst);
    //void exec_store(TileAddress &src);
    
    // void exec_sum(std::vector<bool> &mask, LocalAddress &dst);
    // void exec_sub(std::vector<bool> &mask1, std::vector<bool> &mask2, LocalAddress &dst);
    //void exec_mul(LocalAddress &src1, LocalAddress &src2, LocalAddress &dst);
    //void exec_shift_r(LocalAddress &src, LocalAddress &dst, int imm);
    //void exec_shift_l(LocalAddress &src, LocalAddress &dst, int imm);
    //void exec_mask(LocalAddress &src, LocalAddress &dst, int imm);
    //void exec_lut(LocalAddress &src, LocalAddress &dst);

    //std::vector<int> reduce_sum_exec(Instruction &inst);
    //void global_write(GlobalAddress &addr, std::vector<int> data);

    void exec(Instruction inst, int clock) {
        run_able(inst.pe_mask_, clock);
        if(inst.op_code_ == Instruction::DOT) {
            exec_dot(inst.row_mask_, inst.pe_mask_);
        }
        else if(inst.op_code_ == Instruction::MOV) {
            exec_mov(inst.src1_, inst.dst_);
        }
        else {
            assert_mag(0, "instruction dotn't reconginzed");
        }
    }


    void run(int clock) {
        Instruction inst = inst_buf_.get(pc);
        if(inst.op_code_ == Instruction::HALT) {
            STATE = WAIT;
        }
        else if(STATE == INIT) {
            begin_clock_ = clock;
            if(inst.cycles_ == 1) {
                exec(inst);
                STATE = INIT; pc++;
            }
            else {
                STATE == RUNNING;
            }
        }
        else if(STATE == RUNNING) {
            if(begin_clock_ + inst.cycles_ - 1 == clock) {
                exec(inst);
                STATE = INIT; pc++;
            }
        }
        else {
            assert_msg(0, "run all");
        }
    }
};