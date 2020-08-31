#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>


class DAC {
private:
    static double analog_max;
public:
    static double propagate(int inp);
};

class DACArray {
public:
    static std::vector<double> propagate(vector<int> inp) {
        for(auto in : inp) {
            dacs[]
        }
    }
};


class ADC {
  
};

class Reg {

};

class CrossBar {
private:
    typedef float conductance_t;
    typedef float analog_t; 
    std::vector<std::vector<conductance_t>> matrix_;
    size_t col_size;
    size_t row_size;

public:
    std::vector<int> dot(std::vector<int> &in, std::vector<bool> &mask) {
        std::vector<int> res(col_size, 0);
        for(size_t i = 0; i < col_size; ++i) {
            if(mask[i] == true) {
                for(size_t j = 0; j < row_size; ++j) {
                    res[i] += matrix_[i][j] * in[j];
                }
            }
        }
        return res;
    }
    std::vector<int> element_wise_mul(std::vector<int> &in1, std::vector<bool> &mask) {
        std::vector<int> res(col_size, 0);
        for(size_t i = 0; i < col_size; ++i) {
            for(size_t j = 0; j < row_size; ++j) {
                if(mask[j] == true) {
                    res[i] == matrix_[i][j] * in[j];
                }
            }
        }
        return res;
    }

    std::vector<int> exec_sum(std::vector<bool> &mask) {
        std::vector<int> cb_input(row_size, 0);
        for(size_t i = 0; i < mask.size(); ++i) {
            if(mask[i]) {
                cb_input[i] = 1;
            }
            else {
                cb_input[i] = 0;
            }
        }
        std::vector<int> res = dot(cb_input, std::vector<bool>(col_size_, true));
        return res;
    }
};