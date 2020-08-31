#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>


class LocalAddress {
public:
    bool on_reg;// 7bit, 128 * (128*2/8byte)
    bool on_cb; // 7bit, 128
    int address; 
};

class GlobalAddress {
public:
    int tile; // 12bit, 4096
    int array;// 6bit, 64
    int row;// 7bit, 128
};

class Instruction {
public:
    enum {
        ADD = 1,
        DOT = 2,
        MUL,
        SUB,
        SHIFT_L,
        SHIFT_R,
        MASK,

        MOV,
        MOVS,
        MOVI,
        MOVG,
        LUT,
        REDUCE_SUM
    };
    static int cycle[40];
    int op_code_;
    int cycles_;
    LocalAddress src1_;
    LocalAddress src2_;
    LocalAddress dst_;
    std::vector<bool> reg_mask_;
    std::vector<bool> row_mask_;
    std::vector<bool> col_mask_;
    int imm_;
    GlobalAddress gaddr1_;
    GlobalAddress gaddr2_;
    
};