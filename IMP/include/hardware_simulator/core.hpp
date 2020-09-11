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


// systolic array or SIMD??


class SharedBusCore {
    // SIMD exec model
    std::vector<PE> pe_array_;
    std::vector<ShiftAdd> core_saa_;
    
    InstrBuffer inst_buf_;
    
    RegFile core_buf_;
    
    LUT lut_; // or ALU, VFU, which is better?
    
    SharedBus bus_;
    
    bool halt_;
    // address :
    /*
        pe_input : 0 ~ pe_array_.size * {input_reg_size * (input_reg_bit_/8)}
    */
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

    size_t cross_bar_store_num_bits_;

    size_t dac_resolution_;
    int write_crossbar_cycles;

    size_t pe_input_reg_size;

    size_t pe_output_reg_size;
    
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
    
    DigitalBundle read_data_from_reg(LocalAddress &src, int len) {
        DigitalBundle res(len, 8);
        int addr = src.address;
        if(addr < pe_input_reg_size * pe_array_.size()) {
            for(auto i : range(addr, addr + len)) {
                res[i - addr] = pe_array_[addr / pe_input_reg_size].read_input_reg(i % pe_input_reg_size);
            }
        }
        else if(addr < pe_input_reg_size * pe_array_.size() + pe_output_reg_size * pe_array_.size()) {
            addr = addr - pe_input_reg_size * pe_array_.size();
            for(auto i : range(addr, addr + len)) {
                res[i - addr] = pe_array_[addr / pe_output_reg_size].read_output_reg(i % pe_output_reg_size);
            }
        }
        else { // core buffer 
            addr = addr - pe_input_reg_size * pe_array_.size() - pe_output_reg_size * pe_array_.size();
            for(auto i : range(addr, addr + len)) {
                res[i - addr] = core_buf_.read(i);
            }
        }
        return res;
    }
    
    DigitalBundle read_data_from_crossbar(LocalAddress &src, int len, int clock) {
        // crossbar address according row and col;
        int addr = src.address;
        assert_msg(!locks.is_lock(addr / row_size_, clock), "locked when read");
        
        DigitalBundle res(len, cross_bar_store_num_bits_);

        for(auto i : range(0, len)) {
            res[i] = pe_array_[addr / row_size_].read_crossbar_dummy(addr % row_size_, i));
        }
        return res;
    }

    void write_data_to_reg(LocalAddress &dst, Digital_t data) {
        int addr = dst.address;
        if(addr < pe_input_reg_size * pe_array_.size()) {
            pe_array_[addr / pe_input_reg_size].write_input_reg(addr % pe_input_reg_size, data);
        }
        else if(addr < pe_input_reg_size * pe_array_.size() + pe_output_reg_size * pe_array_.size()) {
            addr = addr - pe_input_reg_size * pe_array_.size();
            pe_array_[addr / pe_output_reg_size].write_output_reg(addr % pe_output_reg_size, data);
        }
        else {
            addr = addr - pe_input_reg_size * pe_array_.size() - pe_output_reg_size * pe_array_.size();
            core_buf_.write(addr, data);
        }
    }

    void write_data_to_crossbar(LocalAddress &dst, DigitalBundle &data, std::vector<bool> mask, int clock) {
        int addr = dst.address;
        assert_msg(!locks.is_lock(addr / row_size_, clock), "locked!!");
        locks.lock(addr / row_size_, clock, write_crossbar_cycles);
        
        DigitalBundle wd(col_size_);
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
        pe_array_[addr / row_size_].write_crossbar(addr % row_size_, wd);
    }

    DigitalBundle read_data(LocalAddress &src, int len, int clock) {
        if(src.on_reg) return read_data_from_reg(src, len);
        if(src.on_cb) return read_data_from_crossbar(src, len, clock);
        assert_msg(0, "local address outof bound");
        return DigitalBundle();
    }

    void write_data(LocalAddress &dst, DigitalBundle &data, std::vector<bool> mask, int clock) {
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

    Core() : row_size_(128), col_size_(128), write_crossbar_cycles(5), pe_array_(8),  dac_resolution_(2),
            pe_input_reg_size(128 * 32 / 8), pe_output_reg_size(128 * 2 / 32 * 32 / 8), reg_buf_size(4096) {}

    Core(Config &config) {}

    void load_instruction();

    void test_add_instr(Instruction inst) {
        inst_buf_.add(inst);
    }

    void exec_dot(std::vector<bool> &pe_mask) {
        assert_msg(pe_mask.size() == pe_array_.size(), "Instruction error");
        for(auto i : range(0, pe_mask.size())) {
            if(pe_mask[i]) { 
                pe_array_[i].exec_inner_product();
            }
        }
    }
    
    void exec_mov(LocalAddress &src, LocalAddress &dst, int len, int clock) {
        DigitalBundle data = read_data(src, len, clock);
        write_data(dst, data, clock);
    }

    void exec_nop() {
        
    }

    void exec_halt() {
        halt_ = true;
    }

    void exec_movi(LocalAddress &dst, int imm, int clock) {
        write_data(dst, Digital_t(imm, 32), clock);
    }

    void exec_movs(LocalAddress &src, LocalAddress &dst, int len, std::vector<bool> &mask) {
        assert_msg(mask.size() == len, "movs instruction format error");
        DigitalBundle src = read_data_from_reg(src, len);
        for(auto idx : range(0, len)) {
            if(mask[idx]) {
                write_data_to_reg(dst + idx, src[idx]);
            }
        }
    }

    //void exec_load(TileAddress &src, LocalAddress &dst);
    //void exec_store(TileAddress &src);
    
    // void exec_sum(std::vector<bool> &mask, LocalAddress &dst);
    // void exec_sub(std::vector<bool> &mask1, std::vector<bool> &mask2, LocalAddress &dst);
    //void exec_mul(LocalAddress &src1, LocalAddress &src2, LocalAddress &dst);
    
    void exec_shift_r(LocalAddress &src, LocalAddress &dst, int shift_bit, int clock) {
        DigitalBundle src = read_data_from_reg(src, 1);
        DigitalBundle src2(0, 0);
        DigitalBundle dst = read_data_from_reg(dst, 1);
        core_saa_[0].power_on(src2, src, shift_bit);
        write_data_to_reg(dst, core_saa_[0].get_data());
    }

    void exec_shift_l(LocalAddress &src, LocalAddress &dst, int shift_bit, int clock) {
        DigitalBundle src = read_data_from_reg(src, 1);
        DigitalBundle src2(0, 0);
        DigitalBundle dst = read_data_from_reg(dst, 1);
        core_saa_[0].power_on(src2, src, shift_bit);
        write_data_to_reg(dst, core_saa_[0].get_data());
    }

    void exec_shift_add(LocalAddress &src1, 
        LocalAddress &src2, LocalAddress &dst, int len, int shift_bit) 
        {
        DigitalBundle src1 = read_data_from_reg(src1, len);
        DigitalBundle src2 = read_data_from_reg(src2, len);
        for(auto idx : range(0, core_saa_.size())) {
            core_saa_[idx].power_on(src1, src2, shift_bit);
            write_data_to_reg(dst + idx, core_saa_[idx].get_data());
        }
    }

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