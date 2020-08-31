#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>
#include "hardware_simulator/memory_array.hpp"
#include "hardware_simulator/isa.hpp"
#include "hardware_simulator/isa.hpp"
#include "hardware_simulator/config.hpp"

class SharedBus {

};

class LUT {
    typedef entry_t int;
    std::vector<entry_t> entries_;
public:
    entry_t lookup(int index) {
        return enrties_[index];
    }
};

class Cluster {
    std::vector<CrossBar> crossbars_;
    std::vector<Instruction> inst_buf_;
    Instruction inst_running_; 
    int begin_clock_;
    LUT lut_;
    SharedBus bus_;
    
    size_t row_size_;
    size_t col_size_;
    
    void write_to_dst(std::vector<int> &data, LocalAddress &dst);
    
    std::vector<int> read_from_src(LocalAddress &src);
    std::vector<int> read_from_reg_mask(std::vector<bool> &reg_mask);
    std::vector<bool> generate_mask_from_address(LocalAddress &addr);

    std::vector<int> get_one_pe_row_data(int index, int row);

    int running_inst_index;
    Instruction get_next_inst();
public:
    Cluster(Config &config);

    void load_instruction();

    void exec_sum(std::vector<bool> &mask, LocalAddress &dst);
    void exec_sub(std::vector<bool> &mask1, std::vector<bool> &mask2, LocalAddress &dst);
    void exec_dot(std::vector<bool> &mask1, std::vector<bool> &reg_mask2, LocalAddress &dst);
    void exec_mul(LocalAddress &src1, LocalAddress &src2, LocalAddress &dst);
    void exec_shift_r(LocalAddress &src, LocalAddress &dst, int imm);
    void exec_shift_l(LocalAddress &src, LocalAddress &dst, int imm);
    void exec_mask(LocalAddress &src, LocalAddress &dst, int imm);
    void exec_lut(LocalAddress &src, LocalAddress &dst);
    void exec_mov(LocalAddress &src, LocalAddress &dst);
    void exec_movi(LocalAddress &dst, int imm);
    void exec_movs(LocalAddress &src, LocalAddress &dst, std::vector<bool> &mask);
    std::vector<int> reduce_sum_exec(Instruction &inst);
    void global_write(GlobalAddress &addr, std::vector<int> data);

    void exec(Instruction inst);

    void run(int clock, bool global_sync, std::vector<int> &comm_data, bool &need_sync);
};