#pragma once
#include <vector>
#include <cstdint>
#include <functional>

#include "data_structure/shape.hpp"
union ReturnType {
    bool bool_;
    int int32_;
    float float32_;
};

class FlatData {
private:
    std::vector<bool> bool_data_;
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

    FlatData(std::int32_t &int32_data) : type_("int32"), size_(1) ,int32_data_(1, int32_data) {}
    FlatData(std::int32_t &int32_data, int size_) : type_("int32"), size_(size_) ,int32_data_(size_, int32_data) {}

    FlatData(std::vector<bool> &bool_data) : type_("bool"), bool_data_(bool_data) {}
    FlatData(std::vector<std::int8_t> &int8_data) : type_("int8"), int8_data_(int8_data) {}
    FlatData(std::vector<std::int16_t> &int16_data) : type_("int16"), int16_data_(int16_data) {}
    FlatData(std::vector<std::int32_t> &int32_data) : type_("int32"), int32_data_(int32_data) {}
    FlatData(std::vector<std::int64_t> &int64_data) : type_("int64"), int64_data_(int64_data) {}

    FlatData(std::vector<float> &float32_data) : type_("float32"), float32_data_(float32_data) {}
    FlatData(std::vector<double> &float64_data) : type_("float64"), float64_data_(float64_data) {}

    ReturnType get_index(int i) {
        ReturnType re;
        if(type_ == "bool") re.bool_ = bool_data_[i];
        if(type_ == "int32")re.int32_ = int32_data_[i];
        if(type_ == "float32")re.int32_ = float32_data_[i];
        return re;
    }
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

    Shape get_shape() const { return shape_; }
    
    void operator =(const NDArray &b) {
        data_ = b.data_;
        shape_ = b.shape_;
        begin_ = b.begin_;
        memory_size_ = b.memory_size_;
    }
    
    bool reshape(Shape shape) {
        shape_ = shape;
        return true;
    }
    
    NDArray operator[](int index) {
        if(shape_.get_dimensions() == 1) {
            return data_->get_index(begin + index);
        }
        int dim = shape_.get_outer_dimension();
        int block_size = memory_size_ / dim;
        Shape shape = shape_.get_inner_shape();
        return NDArray(shape, data_, begin_ + block_size * index, block_size);
    }

    template<class T1, class T2, class T3>
    NDArray element_wist_op(NDArray &b, std::function<T3 (T2, T1)> op) {
        assert_msg(get_shape() == b.get_shape(), "element wist op on ndarry must be same shape(border casted)");
        FlatData* c_data = new FlatData(std::vector<T3>(memory_size_));
        Shape c_shape = b.get_shape();
        for(int i = 0; i < memory_size_; ++i) {
            ReturnType x = data_->get_index(i + begin_);
            ReturnType y = b.data_->get_index(b.begin_+i);
            if(typeid(bool) == T1) {
                c_data[i] = op(x.bool_, y.bool_);
            }
            if(typeid(int) == T1) {
                c_data[i] = op(x.int32_, y.int32_);
            }
            if(typeid(float) == T1) {
                c_data[i] = op(x.float32_, y.float32_);
            }
        }
        return NDArray(c_shape, c_data, 0, memory_size_);
    }

    template<class T1, class T2>
    NDArray unary_op(std::function<T3 (T1)> op) {
        FlatData* c_data = new FlatData(std::vector<T2>(memory_size_));
        Shape c_shape = get_shape();
        for(int i = 0; i < memory_size_; ++i) {
            ReturnType x = data_->get_index(i + begin_);
            if(typeid(bool) == T1) {
                c_data[i] = op(x.bool_);
            }
            if(typeid(int) == T1) {
                c_data[i] = op(x.int32_);
            }
            if(typeid(float) == T1) {
                c_data[i] = op(x.float32_);
            }
        }
        return NDArray(c_shape, c_data, 0, memory_size_);
    }

    template<class T1>
    NDArray transpose() {
        assert_msg(shape_.get_shape_size() == 2, "transpose should apply to matrix(TODO)");
        FlatData* c_data = new FlatData(std::vector<T3>(memory_size_));
    }
};
