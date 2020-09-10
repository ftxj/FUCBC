#define CATCH_CONFIG_MAIN
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <functional>
#include <vector>
#include "catch.hpp"
#include "hardware_simulator/pe.hpp"
#include "hardware_simulator/config.hpp"
#include "test_baseline.hpp"
using namespace std;
//int main() {

TEST_CASE("Inner Product") {
    PE cb;
    Config c;
    
    c.crossbar_rows_ = 16;
    c.crossbar_cols_ = 16;
    c.crossbar_cell_width_ = 2;
    c.crossbar_ou_rows_ = 4;
    c.crossbar_ou_cols_ = 4;
    c.crossbar_represent_number_bits_ = 8;
    c.dac_resolution_ = 1;
    c.adc_resolution_ = 8;
    c.input_reg_size_ = 1024;
    c.input_reg_data_width_ = 8;
    c.output_reg_size_ = 1024;
    c.output_reg_data_width_ = 32;

    srand(time(NULL));
    SECTION("Absolute Address Gen") { 
        IndexDecoder iddc;
        for(int clock = 0; clock <= 128; ++clock) {
            std::vector<int> decomposed_input_values(128);
            fill_int_vector_with_random_number(decomposed_input_values, 1);
            REQUIRE(wlg.power_on(decomposed_input_values) == baseline_dot(matrix, inp));
        }
    }
}
//    return 0;
//}