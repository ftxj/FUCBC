#pragma once
#include <vector>
#include <cstdint>
#include <functional>
#include <memory>
#include "data_structure/shape.hpp"
#include "error/assert.hpp"
template<typename T>
class FlatData {
private:
    std::vector<T> data_;
    std::string type_;
    size_t size_;
public:
    FlatData() : size_(0) {}
    FlatData(const int &size) : size_(size) ,data_(size) {}
    FlatData(const int &size, const T &value) : size_(size) ,data_(size, value) {}
    FlatData(const std::vector<T> &data) : size_(data.size()), data_(data) {}
    FlatData(const FlatData<T> &data) : size_(data.size_), data_(data.data_) {}
    T get_index(int i) { return data_[i];}
    size_t get_size() { return size_; }
    std::vector<T> &get_data() {return data_;}
};

template<typename T>
class NDArray {
private:
    Shape shape_;
    std::shared_ptr<FlatData<T>> data_;
    size_t begin_;
    size_t memory_size_;
public:
    NDArray() : shape_(), 
        data_(std::shared_ptr<FlatData<T>>(new FlatData<T>())), 
        memory_size_(0), begin_(0) {} 
    
    NDArray(const T &value) : shape_(1), 
        data_(std::shared_ptr<FlatData<T>>(new FlatData<T>(1, value))), 
        memory_size_(1), begin_(0) {}
    
    NDArray(const Shape &shape) : shape_(shape), 
        data_(std::shared_ptr<FlatData<T>>(new FlatData<T>(shape.get_shape_size()))), 
        memory_size_(shape.get_shape_size()), begin_(0) {}

    NDArray(const Shape &shape, const T &value) : shape_(shape), 
        data_(std::shared_ptr<FlatData<T>>(new FlatData<T>(shape.get_shape_size(), value))), 
        memory_size_(shape.get_shape_size()), begin_(0) {}
    
    NDArray(const NDArray<T> &b) : shape_(b.shape_), 
        data_(std::shared_ptr<FlatData<T>>(new FlatData<T>(b.data_->get_data()))), 
        memory_size_(b.memory_size_), begin_(b.begin_) {}
    
    NDArray(const Shape &shape, std::shared_ptr<FlatData<T>> &data, size_t begin, size_t memory_size) :
        shape_(shape), data_(data), begin_(begin), memory_size_(memory_size) {}

    Shape get_shape() const { return shape_; }
    
    void operator=(const NDArray<T> &b) {
        data_ = b.data_;
        shape_ = b.shape_;
        begin_ = b.begin_;
        memory_size_ = b.memory_size_;
    }
    
    bool reshape(Shape shape) {
        std::shared_ptr<FlatData<T>> c_data(new FlatData<T>(std::vector<T>(shape.get_shape_size())));
        shape_ = shape;
        data_ = c_data;
        memory_size_ = shape.get_shape_size();
        begin_ = 0;
        return true;
    }
    
    NDArray<T> operator[](int index) {
        if(shape_.get_dimensions() == 1) {
            return data_->get_index(begin_ + index);
        }
        int dim = shape_.get_outer_dimension();
        int block_size = memory_size_ / dim;
        Shape shape = shape_.get_inner_shape();
        return NDArray<T>(shape, data_, begin_ + block_size * index, block_size);
    }

    template<class T2, class T3>
    NDArray<T3> element_wist_op(NDArray<T2> &b, std::function<T3 (T, T2)> op) {
        assert_msg(get_shape() == b.get_shape(), "element wist op on ndarry must be same shape(border casted)");
        std::shared_ptr<FlatData<T3>> c_data(new FlatData<T3>(std::vector<T3>(memory_size_)));
        Shape c_shape = b.get_shape();
        for(int i = 0; i < memory_size_; ++i) {
            T x = data_->get_index(i + begin_);
            T2 y = b.data_->get_index(b.begin_+i);
            c_data[i] = op(x, y);
        }
        return NDArray<T3>(c_shape, c_data, 0, memory_size_);
    }

    template<class T2>
    NDArray<T2> unary_op(std::function<T2 (T)> op) {
        std::shared_ptr<FlatData<T2>> c_data(new FlatData<T2>(std::vector<T2>(memory_size_)));
        Shape c_shape = get_shape();
        for(int i = 0; i < memory_size_; ++i) {
            T x = data_->get_index(i + begin_);
            c_data[i] = op(x);
        }
        return NDArray<T2>(c_shape, c_data, 0, memory_size_);
    }

    NDArray<T> transpose() {
        assert_msg(shape_.get_shape_size() == 2, "transpose should apply to matrix(TODO)");
        std::shared_ptr<FlatData<T>> c_data(new FlatData<T>(std::vector<T>(memory_size_)));
        for(int i = 0; i < shape_.get_ith_dimension(0); ++i) {
            for(int j = 0; i < shape_.get_ith_dimension(1); ++i) {
                c_data[i * shape_.get_ith_dimension(0) + j] = data_[begin_ + j * shape_.get_ith_dimension(1) + i];
            }
        }
        Shape c_shape(shape_.get_ith_dimension(1),shape_.get_ith_dimension(0));
        return NDArray<T>(c_shape, c_data, 0, memory_size_);
    }
};
