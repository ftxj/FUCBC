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
    size_t dac_resolution_;

    class Lock {
        std::vector<int> locked_;
    public:
        Lock() : locked_(8) {}
        Lock(int size) : locked_(size, 0) {}
        void lock(int id, int clock, int write_crossbar_cycles) { 
            assert_msg(id < locked_.size(), "lock out of bound");
            locked_[id] = clock + write_crossbar_cycles; 
            std::cout << "lock on" << id << ", " << clock + write_crossbar_cycles << std::endl;
        }
        bool is_lock(int id, int clock) {
            assert_msg(id < locked_.size(), "lock out of bound"); 
            return locked_[id] >= clock; 
        }
        int lock_id_clock(int id) {
            return locked_[id];
        }
    };
    Lock locks;

    
    size_t row_size_;
    size_t col_size_;
    int write_crossbar_cycles;
    int input_buf_size;
    int output_buf_size;
    int reg_buf_size;

    int ou_size_rows_;
    int ou_size_cols_;

    int pc;
    int STATE;
    enum {
        RUNNING,
        WAIT,
        INIT
    };
    int begin_clock_;
    
    std::vector<Digital_t> read_data_from_reg(LocalAddress &src, int len) {
        std::vector<Digital_t> res(len);
        int addr = src.address;
        if(addr < input_buf_size * pes_.size()) {
            for(auto i : range(addr, addr + len)) {
                res[i - addr] = pes_[addr / input_buf_size].read_input(i % input_buf_size);
            }
        }
        else if(addr < input_buf_size * pes_.size() + output_buf_size * pes_.size()) {
            addr = addr - input_buf_size * pes_.size();
            for(auto i : range(addr, addr + len)) {
                res[i - addr] = pes_[addr / output_buf_size].read_input(i % output_buf_size);
            }
        }
        else {
            addr = addr - input_buf_size * pes_.size() - output_buf_size * pes_.size();
            for(auto i : range(addr, addr + len)) {
                res[i - addr] = reg_.read(i);
            }
        }
        return res;
    }
    
    std::vector<Digital_t> read_data_from_crossbar(LocalAddress &src, int len, int clock) {
        int addr = src.address;
        assert_msg(!locks.is_lock(addr / row_size_, clock), "locked when read");
        std::vector<Digital_t> res;
        for(auto i : range(0, len)) {
            res.push_back(pes_[addr / row_size_].read_crossbar(addr % row_size_, i));
        }
        return res;
    }

    void write_data_to_reg(LocalAddress &dst, Digital_t data) {
        int addr = dst.address;
        if(addr < input_buf_size * pes_.size()) {
            pes_[addr / input_buf_size].write_input(addr % input_buf_size, data);
        }
        else if(addr < input_buf_size * pes_.size() + output_buf_size * pes_.size()) {
            addr = addr - input_buf_size * pes_.size();
            pes_[addr / output_buf_size].write_input(addr % output_buf_size, data);
        }
        else {
            addr = addr - input_buf_size * pes_.size() - output_buf_size * pes_.size();
            reg_.write(addr, data);
        }
    }

    void write_data_to_crossbar(LocalAddress &dst, std::vector<Digital_t> data, std::vector<bool> mask, int clock) {
        int addr = dst.address;
        assert_msg(!locks.is_lock(addr / row_size_, clock), "locked!!");
        locks.lock(addr / row_size_, clock, write_crossbar_cycles);
        std::vector<Digital_t> wd(col_size_);
        std::cout << mask.size() << std::endl;
        int j =0;
        int bit_len = col_size_ / mask.size();
        for(int i = 0; i < mask.size(); ++i) {
            if(mask[i]) {
                for(auto bits : range(0, bit_len)) {
                    wd[i * bit_len + bits] = data[j].split(bits * dac_resolution_, dac_resolution_);
                }
                j++;
            }
            else {
                for(auto bits : range(0, bit_len)) {
                    wd[i * bit_len + bits ] = Digital_t(0, dac_resolution_);
                }
            }
        }
        std::cout << "\n";
        for(auto i : wd) {
            i.print();
            std::cout << " ";
        }
        std::cout << "\n";
        pes_[addr / row_size_].write_crossbar(addr % row_size_, wd);
    }

    std::vector<Digital_t> read_data(LocalAddress &src, int len, int clock) {
        if(src.on_reg) return read_data_from_reg(src, len);
        if(src.on_cb) return read_data_from_crossbar(src, len, clock);
        assert_msg(0, "local address outof bound");
        return std::vector<Digital_t>();
    }

    void write_data(LocalAddress &dst, std::vector<Digital_t> &data, std::vector<bool> mask, int clock) {
        if(dst.on_reg) return write_data_to_reg(dst, data[0]);
        if(dst.on_cb) return write_data_to_crossbar(dst, data, mask, clock);
        assert_msg(0, "local address outof bound");
    }


    void run_able(std::vector<bool> &pe_mask, int clock) {
        for(auto i : range(0, pe_mask.size())) {
            assert_msg(!locks.is_lock(i, clock), "locked!! check your code in cycle=" << locks.lock_id_clock(i));
        }
    }
public:

    Core() : row_size_(128), col_size_(128), write_crossbar_cycles(5), pes_(8),  dac_resolution_(2),
            input_buf_size(128 * 32 / 8), output_buf_size(128 * 2 / 32 * 32 / 8), reg_buf_size(4096) {}

    Core(Config &config) {}

    void load_instruction();

    void test_add_instr(Instruction inst) {
        inst_buf_.add(inst);
    }

    void exec_dot(std::vector<bool> &mask1, std::vector<bool> &mask2) {
        assert_msg(mask1.size() == pes_.size(), "Instruction error");
        assert_msg(mask2.size() == row_size_, "Instruction error");
        for(auto i : range(0, pes_.size())) {
            if(mask1[i]) pes_[i].exec_inner_product(mask2, 32);
        }
    }
    
    void exec_mov(LocalAddress &src, LocalAddress &dst, std::vector<bool> mask, int len, int clock) {
        std::vector<Digital_t> data = read_data(src, len, clock);
        write_data(dst, data, mask, clock);
        
    }

    void exec_nop() {

    }

    void exec_halt() {

    }

    void exec_movi(LocalAddress &dst, int imm, int clock) {
        write_data_to_reg(dst, Digital_t(imm, 32));
    }
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
        if(inst.op_code_ == Instruction::DOT) {
            run_able(inst.pe_mask_, clock);
            exec_dot(inst.pe_mask_, inst.row_mask_);
        }
        else if(inst.op_code_ == Instruction::MOV) {
            std::cout << "mov mask size=" <<  inst.col_mask_.size() <<std::endl;
            exec_mov(inst.src1_, inst.dst_, inst.col_mask_, inst.imm_, clock);
        }
        else if(inst.op_code_ == Instruction::MOVI) {
            exec_movi(inst.dst_, inst.imm_, clock);
        }
        else if(inst.op_code_ == Instruction::NOP) {
            exec_nop();
        }
        else if(inst.op_code_ == Instruction::HALT) {
            exec_halt();
        }
        else {
            assert_msg(0, "instruction dotn't reconginzed");
        }
    }


    void run(int clock, int &halt) {
        Instruction inst = inst_buf_.get(pc);
        std::cout << clock <<  "-" << inst.name << "-" << pc << "-"<< inst.cycles_ << std::endl;
        assert_msg(inst.name != "", "???");
        if(inst.op_code_ == Instruction::HALT) {
            STATE = WAIT;
            halt = 1;
        }
        else if(STATE == INIT) {
            begin_clock_ = clock;
            if(inst.cycles_ == 1) {
                exec(inst, clock);
                STATE = INIT; pc++;
            }
            else {
                STATE = RUNNING;
            }
        }
        else if(STATE == RUNNING) {
            if(begin_clock_ + inst.cycles_ - 1 == clock) {
                exec(inst, clock);
                STATE = INIT; pc++;
                begin_clock_ = clock;
            }
            else if(begin_clock_ + inst.cycles_ - 1 < clock) {
                assert_msg(0, "run error");    
            }
        }
        else {
            assert_msg(0, "run all");
        }
    }
};