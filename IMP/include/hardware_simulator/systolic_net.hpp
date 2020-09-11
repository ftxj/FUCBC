#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>
#include "hardware_simulator/pe.hpp"
#include "hardware_simulator/isa.hpp"
#include "hardware_simulator/config.hpp"
#include "hardware_simulator/digital.hpp"
#include "error/assert.hpp"

template<typename CORE>
class Systolic {
    std::vector<std::vector<CORE>> array_;
public:
    
};