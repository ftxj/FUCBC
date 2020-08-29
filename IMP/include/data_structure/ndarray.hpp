#pragma once
#include <vector>
#include <cstdint>
#include "data_structure/shape.hpp"

class NDArray {
private:
    typedef union supported_types_ {
        std::int32_t n;
        std::uint16_t s; 
        std::uint8_t c;
    } ValueType;

    std::string type_;
    std::vector<ValueType> data_;
    Shape shape_;
    size_t begin_;
    size_t memory_size_;
public:
    NDArray(std::int32_t value) : shape_(1), data_(1, value), memory_size_(1) {}
    NDArray(Shape shape) : shape_(shape) {
        // int dim = shape.get_dimensions();
        // assert(dim > 0);
        // memory_size_ = 1;
        // for(auto i : range(0, dim)) {
        //     memory_size_ = memory_size_ * get_ith_dimension(i);
        // }
    }
    NDArray(const NDArray &b) : shape_(b.shape_), data_(a.data_) {}
    void operator =(const NDArray<ValueType> &b) {
        data_ = b.data_;
        shape_ = b.shape_;
    }
    //NDArray& operator[](int index);
};

