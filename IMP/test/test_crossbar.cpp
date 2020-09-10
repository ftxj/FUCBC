#define CATCH_CONFIG_MAIN
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <functional>
#include <vector>
#include "catch.hpp"
#include "hardware_simulator/crossbar.hpp"
#include "test_baseline.hpp"
using namespace std;
//int main() {
    

TEST_CASE("Inner Compute are compute") {
    CrossBar cb;
    SECTION("write and read function test") {
        for(int mnum = 1; mnum <= 128; ++mnum) {
            std::vector<std::vector<float>> matrix(mnum, std::vector<float>(128));
            std::vector<float> inp(128, 1);
            fill_matrix_with_random_number(matrix, 3.0);
            for(int i = 0; i < mnum - 1; ++i) {
                cb.write_row(i, matrix[i]);
            }
            cb.write_row(127, matrix[mnum - 1]);

            for(int i = 0; i < mnum - 1; ++i) {
                REQUIRE(cb.read(i) == matrix[i]);
            }
            REQUIRE(cb.read(127) == matrix[mnum - 1]);
            AnalogBundle ainp(inp);
            //cb.dot(ainp).dump(std::cout);
            REQUIRE(cb.dot(ainp) == baseline_dot(matrix, inp));
            cb.clear();
        }
    }
}
//    return 0;
//}