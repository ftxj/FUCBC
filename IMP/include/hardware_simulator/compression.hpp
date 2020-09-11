#pragma once
#include <vector>

void copy_ou(std::vector<std::vector<float>> &in, std::vector<std::vector<float>> &out, 
    int in_row, int in_col, int out_row, int out_col, 
    size_t ou_row, size_t ou_col) {
        for(int j = 0; j < ou_col; ++j) {
            out[out_row][out_col + j] = in[in_row][in_col + j];
        }
}


// give dense matrix rep, return index encoding and sparse OU base matrix
std::pair<std::vector<std::vector<float>>, std::vector<int>> compression(std::vector<std::vector<float>> &in, size_t row_size, size_t ou_row, size_t ou_col) {
    
    std::vector<std::vector<float>> res(in.size(), std::vector<float>(in[0].size()));

    std::vector<std::vector<int>> input_address(in[0].size() / ou_col);
    
    int out_ou_col_index = 0;
    
    int out_ou_row_index = 0;

    for(auto j = 0; j < in[0].size() / ou_col; ++j) {
        for(auto i = 0; i < in.size() / ou_row; ++i) {

            for(auto oi = 0; oi < ou_row; ++oi) {
                for(auto oj = 0; oj < ou_col; ++oj) {
                    // [i, j] block
                    if(in[i * ou_row + oi][j * ou_col + oj] != 0) {
                        // this row not all zero
                        input_address[j].push_back(i * ou_row + oi);
                        
                        copy_ou(in, res, 
                            i * ou_row + oi, j * ou_col, 
                            out_ou_row_index, j * ou_col, 
                            ou_row, ou_col
                        );
                        out_ou_row_index++;
                        break;
                    }
                }
            }
        }
        out_ou_col_index++;
        out_ou_row_index=0;
    }
    std::vector<int> res2;
    for(auto i : input_address) {
        for(int k = 0; k < row_size; ++k) {
            if(k >= i.size()) res2.push_back(0);
            else if(k == 0) res2.push_back(i[k]);
            else res2.push_back(i[k] - i[k-1]);
        }
    }
    return {res, res2};
}


std::vector<int> de_pack(int data, int in_bits, int out_bits) {
    std::vector<int> res(in_bits / out_bits);
    for(int i = 0; i < in_bits / out_bits; ++i) {
        int mask = (1 << out_bits) - 1;
        res[res.size() - 1 - i] = (data >> (i*out_bits)) & mask;
    }
    return res; 
}

std::vector<std::vector<float>> matrix_in_out(std::vector<std::vector<int>> &in, size_t in_bits, size_t out_bits) {
    std::vector<std::vector<float>> res(in.size(), std::vector<float>(in[0].size() * (in_bits / out_bits)));
    for(int i = 0; i < in.size(); ++i) {
        for(int j = 0; j < in[0].size(); ++j) {
            auto pack = de_pack(in[i][j], in_bits, out_bits);
            for(int k = 0; k < pack.size(); ++k) {
                res[i][j * pack.size() + k] = pack[k];
            }
        }
    }
    return res;
}

std::vector<std::vector<std::vector<float>>> suit_crossbar(std::vector<std::vector<float>> &in, size_t crossbar_row, size_t crossbar_col) {
    std::vector<std::vector<std::vector<float>>> res;
    for(int row_block = 0; row_block < in.size() / crossbar_row; ++row_block) {
        for(int col_block = 0; col_block < in[0].size() / crossbar_col; ++col_block) {
            std::vector<std::vector<float>> tmp(crossbar_row, std::vector<float>(crossbar_col));
            for(int i = 0; i < crossbar_row; ++i) {
                for(int j = 0; j < crossbar_col; ++j) {
                    tmp[i][j] = in[row_block * crossbar_row + i][col_block * crossbar_col + j];
                }
            }
            for(int i = 0; i < crossbar_row; ++i) {
                for(int j = 0; j < crossbar_col; ++j) {
                    if(tmp[i][j] != 0) {
                        res.push_back(tmp);
                        i = crossbar_row;
                        break;
                    }
                }
            }
        }
    }
    return res;
}

template<typename T>
void dump(std::vector<T> &in) {
    for(auto i : in) {
        std::cout << i << ",";
    }
    std::cout << "\n";
}

template<typename T>
void dump(std::vector<std::vector<T>> &in) {
    for(auto x : in) {
        dump(x);
    }
}