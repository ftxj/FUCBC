#pragma once
#include <vector>
#include <iostream>
std::vector<int> range(int begin, int end) {
    std::vector<int> iter;
    for(int i = begin; i < end; ++i) {
        iter.push_back(i);
    }   
    return iter;
}

void ftxj_debug_print(std::string s) {
    std::cout << s << std::endl;
}