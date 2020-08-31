#include "hardware_simulator/tile.hpp"
#include "hardware_simulator/isa.hpp"

class Chip {
private:
    std::vector<Tile> tiles_;
    std::vector<std::vector<std::vector<int>>> sync_data_;
    std::vector<std::vector<bool>> sync_mask_;
    int num_tiles_;
    int num_clusters_;
    int num_pe_;
    
    bool sync_;
    int global_addr_trasn_;
    std::vector<GlobalAddress> write_global_buf_;

    bool need_global_compute();
    void global_compute();
    void global_compute_complete();
    void write_data_to_addr(GlobalAddress &addr, std::vector<int> data);
public:
    Chip();
    void init_hardware(Config);
    
    void load_instruction();

    void run(int clock);
};