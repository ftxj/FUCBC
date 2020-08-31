#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>

class Instruction {
public:
    enum {
        ADD = 1,
        DOT = 2
    };
    int op_code_;
    LocalAddress src1_;
    LocalAddress src2_;
    LocalAddress dst_;
    std::vector<bool> reg_mask_;
    std::vector<bool> row_mask_;
    std::vector<bool> col_mask_;
    int imm_;
    int gaddr1_;
    int gaddr2_;
};

class LocalAddress {
public:
    bool on_reg;
    bool on_cb;
    int address;
};