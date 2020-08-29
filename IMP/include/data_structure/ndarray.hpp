#pragma once
#include <vector>
#include <cstdint>
#include "data_structure/shape.hpp"

class FlatData {
private:
    std::vector<std::int8_t> int8_data_;
    std::vector<std::int16_t> int16_data_;
    std::vector<std::int32_t> int32_data_;
    std::vector<std::int64_t> int64_data_;
    std::vector<float> float32_data_;
    std::vector<double> float64_data_;
    std::string type_;
    size_t size_;
public:
    FlatData() : type_("unknow"), size_(0) {}
    FlatData(std::vector<std::int8_t> &int8_data) : type_("int8"), int8_data_(int8_data) {}
    FlatData(std::vector<std::int16_t> &int16_data) : type_("int16"), int16_data_(int16_data) {}
    FlatData(std::int32_t &int32_data) : type_("int32"), int32_data_(1, int32_data) {}
    FlatData(std::vector<std::int32_t> &int32_data) : type_("int32"), int32_data_(int32_data) {}
    FlatData(std::vector<std::int64_t> &int64_data) : type_("int64"), int64_data_(int64_data) {}

    FlatData(std::vector<float> &float32_data) : type_("float32"), float32_data_(float32_data) {}
    FlatData(std::vector<double> &float64_data) : type_("float64"), float64_data_(float64_data) {}
    int get_index(int i) { return int32_data_[i]; }
    size_t get_size() { return size_; }
};

class NDArray {
private:
    Shape shape_;
    FlatData* data_;
    size_t begin_;
    size_t memory_size_;
public:
    NDArray() : shape_(), data_(new FlatData()), memory_size_(0), begin_(0) {} 
    NDArray(std::int32_t value) : shape_(1), data_(new FlatData(value)), memory_size_(1), begin_(0) {}
    NDArray(Shape shape) : shape_(shape), memory_size_(shape.get_shape_size()), begin_(0) {}
    NDArray(const NDArray &b) : shape_(b.shape_), data_(b.data_), memory_size_(b.memory_size_), begin_(b.begin_) {}
    NDArray(Shape &shape, FlatData* data, size_t begin, size_t memory_size) :
        shape_(shape), data_(data), begin_(begin), memory_size_(memory_size) {}

    //NDArray(float value) : type_("float32"), shape_(1), float32_data_(1, value), memory_size_(1) {}
    Shape get_shape() const { return shape_; }
    void operator =(const NDArray &b) {
        data_ = b.data_;
        shape_ = b.shape_;
    }
    bool reshape(Shape shape) {
        shape_ = shape;
        return true;
    }
    NDArray operator[](int index) {
        int dim = shape_.get_outer_dimension();
        int block_size = memory_size_ / dim;
        Shape shape = shape_.get_inner_shape();
        return NDArray(shape, data_, begin_ + block_size * index, block_size);
    }
};

