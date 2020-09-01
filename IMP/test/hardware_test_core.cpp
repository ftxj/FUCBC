#include <iostream>
#include "hardware_simulator/core.hpp"
#include "hardware_simulator/isa.hpp"

#include "util.hpp"
/*
Reg默认地址空间：
128 * 4 * 8, input
8 * 4 * 8, output
4096, reg
CrossBar默认地址空间：
128 * 8,
*/

/*
[
    1 2 3
    4 5 6
    7 8 9
]
[
    10 11 12
]
movi 1, 1
movi 2, 2
movi 3, 3
movi 4, 4
movi 5, 5
movi 6, 6
movi 7, 7
movi 8, 8
movi 9, 9

mov reg1, cb1, {1,1,1,000000}, 3
mov reg4, cb2, {1,1,1,000000}, 3
mov reg7, cb3, {1,1,1,000000}, 3

movi 1, 10
movi 2, 11
movi 3, 12

dot {1,0,0,000}, {1,1,1,00000}
*/

#define MOVI_1_INPUT(dst, imm) core.test_add_instr(MOVI(imm, dst, true, false))
#define MOV_1_INPUT_1_CB_ORDER(reg, cb, len, K) \
    do{\
        std::vector<bool> tmp##K(128, false);\
        for(int i = 0; i < len; ++i) {\
            tmp##K[i] = true;\
        }\
        core.test_add_instr(MOV(reg, true, false, cb, false, true, len, tmp##K));\
    }while(false)


#define DOT(a, len, K) \
    do{\
        std::vector<bool> omp##K(8, false);\
        omp##K[a] = true;\
        std::vector<bool> tmp##K(128, false);\
        for(int i = 0; i < len; ++i) {\
            tmp##K[i] = true;\
        }\
        core.test_add_instr(DOT(omp##K, tmp##K));\
    }while(false)

int main() {
    Core core;
    MOVI_1_INPUT(1, 1);
    MOVI_1_INPUT(2, 2);
    MOVI_1_INPUT(3, 3);
    MOVI_1_INPUT(4, 4);
    MOVI_1_INPUT(5, 5);
    MOVI_1_INPUT(6, 6);
    MOVI_1_INPUT(7, 7);
    MOVI_1_INPUT(8, 8);
    MOVI_1_INPUT(9, 9);

    MOV_1_INPUT_1_CB_ORDER(1, 1, 3, 1);
    MOV_1_INPUT_1_CB_ORDER(4, 2, 3, 2);
    MOV_1_INPUT_1_CB_ORDER(7, 3, 3, 3);

    MOVI_1_INPUT(1, 10);
    MOVI_1_INPUT(2, 11);
    MOVI_1_INPUT(3, 12);

    DOT(1, 3, 4);
    for(int i = 0; i < 100; ++i) {
        core.run(i);
    }
    return 0;
}