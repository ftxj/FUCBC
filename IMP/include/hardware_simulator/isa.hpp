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


class TileAddress {
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
        MOV = 3,
        MUL,
        SUB,
        SHIFT_L,
        SHIFT_R,
        MASK,
        HALT,
        MOVS,
        MOVI,
        MOVG,
        LUT,
        REDUCE_SUM
    };
    std::string name;
    int op_code_;
    int cycles_;
    LocalAddress src1_;
    LocalAddress src2_;
    LocalAddress dst_;

    std::vector<bool> pe_mask_;
    std::vector<bool> reg_mask_;
    std::vector<bool> row_mask_;
    std::vector<bool> col_mask_;
    int imm_;
    GlobalAddress gaddr1_;
    GlobalAddress gaddr2_;
    Instruction() {}
};

class MOVI : public Instruction {
public:
    MOVI(int imm, int src, bool reg, bool cb) : Instruction() {
        op_code_ = Instruction::MOVI;
        imm_ = imm;
        dst_.on_reg = reg;
        dst_.on_cb = cb;
        dst_.address = src;
        name = "MOVI";
        cycles_ = 3;
    } 
};


class MOV : public Instruction {
public:
    MOV(int src, bool reg1, bool cb1, int dst, bool reg2, bool cb2, int len, std::vector<bool> &mask) :Instruction()
     {
         op_code_ = Instruction::MOV;
        imm_ = len;
        src1_.on_reg = reg1;
        src1_.on_cb = cb1;
        src1_.address = src;
    
        dst_.on_reg = reg2;
        dst_.on_cb = cb2;
        dst_.address = dst;

        col_mask_ = mask;
        name = "MOV";
        cycles_ = 3;
    } 
};

class DOT : public Instruction {
public:
    DOT(std::vector<bool> &mask1, std::vector<bool> &mask2) :Instruction() {
        op_code_ = Instruction::DOT;
        row_mask_ = mask1;
        pe_mask_ = mask2;
        name = "DOT";
        cycles_ = 18;
    } 
};