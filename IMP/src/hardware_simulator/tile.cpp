#include "hardware_simulator/tile.hpp"

Tile::Tile() {}

void Tile::init_hardware(Config config) {
    clusters_ = std::vector<Cluster>(config.num_clusters_);
    for(size_t i = 0; i < clusters_.size(); ++i) {
        clusters_[i].init(config);
    }
}

void Tile::load_instruction() {

}

void Tile::global_write(GlobalAddress &addr, std::vector<int> &data) {
    int cluster_index = addr.array >> 3;
    clusters_[cluster_index].global_write(addr, data);
}
