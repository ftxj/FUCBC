#include "hardware_simulator/chip.hpp"

Chip::Chip() {
    
}

void Chip::init_hardware(Config config) {
    tiles_ = std::vector<Tile>(config.num_tiles_);
    
    num_tiles_ = config.num_tiles_;
    num_clusters_ = config.num_clusters_;
    num_pe_ = config_num.num_pe_;

    sync_ = false;

    for(size_t i = 0; i < tiles_.size(); ++i) {
        tiles_[i].init(config);
    }
}

bool Chip::need_global_compute() {
    for(size_t i = 0; i < num_tiles_; ++i) {
        for(size_t j = 0; j < num_clusters_; ++j) {
            if(sync_mask_[i][j] == false) {
                return false;
            } 
        }
    }
    return true;
}

void Chip::global_compute() {
    std::vector<int> res;
    for(size_t i = 0; i < num_tiles_; ++i) {
        for(size_t j = 0; j < num_clusters_; ++j) {
            for(size_t k = 0; k < num_clusters_; ++k) {
                res[k] += sync_data_[i][j][k];
        }
    }
    write_data_to_addr(write_global_buf_[global_addr_trasn_], res);
}

void Chip::global_compute_complete() {
    sync_mask_ = std::vector<std::vector<bool>>(num_tiles_, (num_clusters_, false));
    global_addr_trasn_ ++;
}

void Chip::write_data_to_addr(GlobalAddress &addr, std::vector<int> data) {
    int tile_index = addr.tile;
    tiles_[tile_index].global_write(addr, data);
}

void Chip::run(int clock) {
    for(int t = 0; t < num_tiles_; ++t) {
        std::vector<std::vector<int>> sync_data;
        std::vector<bool> sync_mask;
        tiles_[t].run(clock, sync_, sync_data, sync_mask);
        sync_ = false;
        for(int i = 0; i < num_clusters_; ++i) {
            if(sync_mask[i] == true) {
                sync_mask_[t][i] = true;
                sync_data_[t][i] = sync_data;
            }
        }
    }
    if(need_global_compute()) {
        global_compute();
        global_compute_complete();
        sync_ = true;
    }
}