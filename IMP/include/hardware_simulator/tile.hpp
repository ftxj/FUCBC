#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>
#include "hardware_simulator/cluster.hpp"


class XBBus {

};

class InstBuf {

};


class ShiftAdd {
// IMP use 128 bit shift & add 

};

class PC {
    std::vector<ShiftAdd> sas_; // 64 (8 * 8) 个
    int vector_in_;
    int vector_out_;
public:
    std::vector<int> saa(std::vector<int> &in) {

    }
};


class Tile {
    std::vector<Cluster> clusters_;
    XBBus commuciate_;
    InstBuf inst_buf_;
    PC sa_;
public:
    Cluster(const int& crossbar_num) : crossbars_(crossbar_num) {}
    void exec(Instruction inst) {
        if(inst.op_code_ == Instruction::ADD) {
            std::vector<bool> &mask = inst.row_mask_;
            LocalAddress &write_addr = inst.dst_;
            if(write_addr.on_reg) {

            }
            if(write_addr.on_cb) {

            }
            for(auto )
        }
    }
};