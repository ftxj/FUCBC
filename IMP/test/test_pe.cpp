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
#include "hardware_simulator/compression.hpp"
#include "test_baseline.hpp"
using namespace std;
//int main() {

TEST_CASE("Inner Product") {
    Config c;
    c.crossbar_rows_ = 8;
    c.crossbar_cols_ = 8;
    c.crossbar_cell_width_ = 2;
    
    c.crossbar_ou_rows_ = 2;
    c.crossbar_ou_cols_ = 2;
    
    c.crossbar_represent_number_bits_ = 4;
    
    c.dac_resolution_ = 1;
    c.adc_resolution_ = 3;
    
    c.shift_add_width_ = 32;

    c.input_reg_size_ = 1024;
    c.input_reg_data_width_ = 4;
    
    c.output_reg_size_ = 1024;
    c.output_reg_data_width_ = 32;


    PE pe(c);

    std::vector<int> index_1 {0,1,1,1,1,1,1,1};
    std::vector<int> index_2 {1,2,6,0,0,0,0,0};
    std::vector<int> index_3 {0,1,3,4,1,1,1,1};
    std::vector<int> index_4 {1,2,2,3,5,1,0,0};
    std::vector<int> index_in;
    index_in.insert(index_in.end(), index_1.begin(), index_1.end());
    index_in.insert(index_in.end(), index_2.begin(), index_2.end());
    index_in.insert(index_in.end(), index_3.begin(), index_3.end());
    index_in.insert(index_in.end(), index_4.begin(), index_4.end());

    // 0x01, 0x03, 0x0e, 0x08, 0x0c, 0x04, 0x07, 0x0d, 0x0a, 0x07, 0x09, 0x0d, 0x0c, 0x0e, 0x0b, 0x0d
    std::vector<int> input {1,3,14,8,12,4,7,13,10,7,9,13,12,14,11,13};
    
    std::vector<std::vector<float>> weight = {
        {0, 1, 0, 0, 1, 3, 0, 0},   // 1, 0, 7, 0
        {0, 2, 1, 0, 3, 3, 2, 2},   // 2, 4, 15, 10

        {3, 0, 0, 0, 0, 0, 0, 0},   // 12, 0, 0, 0
        {0, 2, 1, 2, 0, 0, 1, 0},   // 2, 6, 0, 4
        
        {2, 0, 0, 0, 3, 0, 0, 0},   // 8, 0, 12, 0
        {0, 1, 0, 0, 0, 0, 3, 0},   // 1, 0, 0, 12
        
        {2, 2, 0, 0, 0, 0, 0, 0},   // 10, 0, 0, 0
        {3, 3, 0, 0, 0, 0, 0, 0},   // 15, 0, 0, 0
        
        {0, 0, 0, 0, 2, 2, 1, 3},   // 0, 0, 10, 7
        {0, 0, 1, 3, 0, 2, 0, 0},   // 0, 7, 2, 0
        
        {0, 0, 0, 0, 1, 1, 0, 0},   // 0, 0, 5, 0
        {0, 0, 0, 0, 3, 1, 0, 0},   // 0, 0, 13, 0
        
        {0, 0, 0, 0, 2, 2, 0, 0},   // 0, 0, 10, 0
        {0, 0, 0, 0, 0, 0, 2, 2},   // 0, 0, 0, 10
        
        {0, 0, 0, 0, 0, 0, 1, 0},   // 0, 0, 0, 4
        {0, 0, 0, 0, 0, 0, 0, 0}    // 0, 0, 0, 0
        
    };
    

    std::vector<std::vector<float>> weight_origin = {
        {1, 0, 7, 0},   // 1, 0, 7, 0
        {2, 4, 15, 10},   // 2, 4, 15, 10

        {12, 0, 0, 0},   // 12, 0, 0, 0
        {2, 6, 0, 4},   // 2, 6, 0, 4
        
        {8, 0, 12, 0},   // 8, 0, 12, 0
        {1, 0, 0, 12},   // 1, 0, 0, 12
        
        {10, 0, 0, 0},   // 10, 0, 0, 0
        {15, 0, 0, 0},   // 15, 0, 0, 0
        
        {0, 0, 10, 7},   // 0, 0, 10, 7
        {0, 7, 2, 0},   // 0, 7, 2, 0
        
        {0, 0, 5, 0},   // 0, 0, 5, 0
        {0, 0, 13, 0},   // 0, 0, 13, 0
        
        {0, 0, 10, 0},   // 0, 0, 10, 0
        {0, 0, 0, 10},   // 0, 0, 0, 10
        
        {0, 0, 0, 4},   // 0, 0, 0, 4
        {0, 0, 0, 0}    // 0, 0, 0, 0
        
    };

    std::vector<std::vector<float>> weight_2 = {
        {0, 1, 1, 0, 1, 3, 2, 2},
        {0, 2, 1, 2, 3, 3, 1, 0}, 

        {3, 0, 1, 3, 3, 0, 3, 0}, 
        {0, 2, 0, 0, 2, 2, 1, 3}, 
        
        {2, 0, 0, 0, 0, 2, 2, 2},  
        {0, 1, 0, 0, 1, 1, 1, 0},  

        {2, 2, 0, 0, 3, 1, 0, 0},  
        {3, 3, 0, 0, 2, 2, 0, 0}
    };

    std::vector<int> res = {

    };

    SECTION("Absolute Address Gen") { 
        
        pe.index_decoder_.load_data(index_in);
        
        std::vector<int> index_res_1 {0,1,2,3,4,5,6,7};
        std::vector<int> index_res_2 {1,3,9,9,9,9,9,9};
        std::vector<int> index_res_3 {0,1,4,8,9,10,11,12};
        std::vector<int> index_res_4 {1,3,5,8,13,14,14,14};
        
        REQUIRE(pe.index_decoder_.power_on(0).to_int_vector() == index_res_1);
        REQUIRE(pe.index_decoder_.power_on(1).to_int_vector() == index_res_2);
        REQUIRE(pe.index_decoder_.power_on(2).to_int_vector() == index_res_3);
        REQUIRE(pe.index_decoder_.power_on(3).to_int_vector() == index_res_4);
    }

    SECTION("Active Value Load") { 
        pe.index_decoder_.load_data(index_in);
        pe.pe_input_.load_data(input, 4);

        auto absolute_address = pe.index_decoder_.power_on(0);
        pe.pe_input_.get_active_values(absolute_address);
        
        std::vector<int> active_res_1_1 {1,1,0,0,0,0,1,1};
        std::vector<int> active_res_1_2 {0,1,1,0,0,0,1,0};
        std::vector<int> active_res_1_3 {0,0,1,0,1,1,1,1};
        std::vector<int> active_res_1_4 {0,0,1,1,1,0,0,1};
        //pe.pe_input_.dump(std::cout);
        REQUIRE(pe.pe_input_.read_rows_data(1, 0).to_int_vector() == active_res_1_1);
        REQUIRE(pe.pe_input_.read_rows_data(1, 1).to_int_vector() == active_res_1_2);
        REQUIRE(pe.pe_input_.read_rows_data(1, 2).to_int_vector() == active_res_1_3);
        REQUIRE(pe.pe_input_.read_rows_data(1, 3).to_int_vector() == active_res_1_4);
    }

    SECTION("Word Line Generator") { 
        pe.index_decoder_.load_data(index_in);
        pe.pe_input_.load_data(input, 4);
        auto absolute_address = pe.index_decoder_.power_on(0);
        pe.pe_input_.get_active_values(absolute_address);

        auto activation_value = pe.pe_input_.read_rows_data(1, 0);
        auto activation_value_1 = pe.pe_input_.read_rows_data(1, 1);
        auto activation_value_2 = pe.pe_input_.read_rows_data(1, 2);
        auto activation_value_3 = pe.pe_input_.read_rows_data(1, 3);

        std::vector<int> ou_row_1 {1,1,0,0,0,0,0,0};
        std::vector<int> ou_row_2 {0,0,0,0,0,0,1,1};

        std::vector<int> ou_row_2_1 {0,1,1,0,0,0,0,0};
        std::vector<int> ou_row_2_2 {0,0,0,0,0,0,1,0};

        std::vector<int> ou_row_3_1 {0,0,1,0,1,0,0,0};
        std::vector<int> ou_row_3_2 {0,0,0,0,0,1,1,0};
        std::vector<int> ou_row_3_3 {0,0,0,0,0,0,0,1};

        
        std::vector<int> ou_row_4_1 {0,0,1,1,0,0,0,0};
        std::vector<int> ou_row_4_2 {0,0,0,0,1,0,0,1};
        
        REQUIRE(pe.word_line_gen_.power_on(activation_value, 0).to_int_vector() == ou_row_1);
        REQUIRE(pe.word_line_gen_.power_on(activation_value, 1).to_int_vector() == ou_row_2);
        
        REQUIRE(pe.word_line_gen_.power_on(activation_value_1, 0).to_int_vector() == ou_row_2_1);
        REQUIRE(pe.word_line_gen_.power_on(activation_value_1, 1).to_int_vector() == ou_row_2_2);
        
        REQUIRE(pe.word_line_gen_.power_on(activation_value_2, 0).to_int_vector() == ou_row_3_1);
        REQUIRE(pe.word_line_gen_.power_on(activation_value_2, 1).to_int_vector() == ou_row_3_2);
        REQUIRE(pe.word_line_gen_.power_on(activation_value_2, 2).to_int_vector() == ou_row_3_3);
        
        REQUIRE(pe.word_line_gen_.power_on(activation_value_3, 0).to_int_vector() == ou_row_4_1);
        REQUIRE(pe.word_line_gen_.power_on(activation_value_3, 1).to_int_vector() == ou_row_4_2);
    }

    SECTION("PE inner produce") { 
        pe.index_decoder_.load_data(index_in);
        pe.pe_input_.load_data(input, 4);
        pe.cross_bar_.load_data(weight_2);
        pe.exec_inner_product();
        REQUIRE(pe.pe_output_.read(0, 4).to_int_vector() == baseline_dot(weight_origin, input));
    }
}
//    return 0;
//}