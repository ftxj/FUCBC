#pragma once
#include <vector>
#include <set>
#include <map>
#include "hardware_simulator/config.hpp"
class DAC {
    int num_access_;
    int latency_;
    int resolution_;
public:
    DAC() : latency_(1), resolution_(2) { DAC::component_number_++; }
    DAC(Config);
    
    int get_latency() { return latency_; }
    int get_resolution() { return resolution_; }

    float bin
    static int component_number_;
};

class DACArray {

};

class ADC {

};

class SimpleHold {

};

class ShiftAdd {

};

class Reg {

};

class RegFile {

};

class LUT {

};

class NoC {

};

class HTree {

};

class InstrBuffer {

};