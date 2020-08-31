#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>
#include "hardware_simulator/cluster.hpp"
#include "hardware_simulator/config.hpp"

class XBBus {

};

// class PC {
//     std::vector<ShiftAdd> sas_; // 64 (8 * 8) 个
//     int vector_in_;
//     int vector_out_;
// public:
//     std::vector<int> saa(std::vector<int> &in) {

//     }
// };

class Tile {
    std::vector<Cluster> clusters_;
    XBBus commuciate_;
public:
    Tile();
    void init_hardware(Config);
    void load_instruction();
    void run(int clock, bool global_sync, std::vector<std::vector<int>> &comm_data, std::vector<bool> &need_sync) {
        for(int i = 0; i < clusters_.size(); ++i) {
            clusters_[i].run(clock, global_sync, comm_data[i], need_sync[i]);
        }
    }

    void global_write(GlobalAddress &addr, std::vector<int> &data);
    
};