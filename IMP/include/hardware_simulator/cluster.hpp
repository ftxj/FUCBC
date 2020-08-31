#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>
#include "hardware_simulator/memory_array.hpp"
#include "hardware_simulator/isa.hpp"

class SharedBus {

};

class LUT {
    typedef entry_t int;
    std::vector<entry_t> entries_;
public:
    entry_t search(int index) {
        return enrties_[index];
    }
};

class RegFile {

};

class Cluster {
    std::vector<CrossBar> crossbars_;
    SharedBus bus_;
    LUT lut_;
    RegFile regs_;
    size_t row_size_;
    size_t col_size_;
public:
    Cluster(const int& crossbar_num) : crossbars_(crossbar_num) {}
    
    std::vector<std::vector<int>> exec_sum(std::vector<bool> &mask) {
        std::vector<std::vector<int>> partial_sum(crossbars_.size());
        for(size_t i = 0; i < crossbars_.size(); ++i) {
            partial_sum[i] = crossbars_[i].exec_sum(mask);
        }
       return partial_sum;
    }
};