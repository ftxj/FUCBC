#include <cmath>
class Config {
    int adc_resolution_needed(int rows, int input_per_cycle, int cell_bit) {
        // from ISAAC
        if(input_per_cycle > 1 && cell_bit > 1) {
            return input_per_cycle + cell_bit + std::log2(rows);
        }
        return input_per_cycle + cell_bit + std::log2(rows) - 1;
    }
public:
    size_t num_tiles_;
    size_t num_clusters_;
    size_t num_pe_;
    
    size_t reram_rows_;
    size_t reram_cols_;
    size_t reram_size_; // 128 * 128
    size_t reram_cell_width_; // 2, 一个 cb 存储 4KB 数据，一个 cb 相当于一个 128 * 8（128*2 / 32）的 SIMD 指令
    size_t reram_per_cluster_;// 8, 128*128*8 = 
    size_t adc_resolution_;// 5, each cross bar has 2 adc
    size_t dac_resolution_; // 2, each cross bar has 2 dac
    size_t input_reg_per_cluster_;// 256 byte = 32 * 8  
    size_t output_reg_per_cluster;// 256 byte = 32 * 8

};