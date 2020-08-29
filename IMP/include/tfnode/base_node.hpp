#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>

#include "data_structure/ndarray.hpp"
#include "data_structure/shape.hpp"

template<typename ValueType>
class Tensor {
private:
    std::string name_;
    std::string dtype_;
    NDArray<ValueType> values_;
    Shape shape_;
public:
    Tensor() : shape_(1), name_("no name"), dtype_("int") {}
    Tensor(std::string name) : shape_(1), name_(name), dtype_("int") {}
    Tensor(std::string name, Shape shape) : shape_(shape), name_(name), dtype_("int") {}
    Tensor(std::string name, Shape shape, std::string dtype) : shape_(shape), name_(name), dtype_(dtype) {}
    Tensor(NDArray<ValueType> values, std::string name, Shape shape, std::string dtype) : values_(values), shape_(shape), name_(name), dtype_(dtype) {}

    std::string get_name() { return name_; }
    std::string get_dtype() { return dtype_; }
    void set_values(NDArray<ValueType> value) { values_ = value; }
    NDArray<ValueType>& get_values() { return values_; }
    Shape get_shape() { return shape_; }
    bool dtype_check(Tensor& t) { return dtype_ == t.dtype_; }
    bool shape_equal(Tensor& t) { return dtype_ == t.dtype_; }
};

template<typename ValueType>
class BaseNode : Tensor<ValueType> {
private:
    std::string type_;

    std::vector<BaseNode*> successors_;
    std::vector<BaseNode*> predecessors_;
public:
    BaseNode() : Tensor() {}
    BaseNode(std::string name) : Tensor(name), type_("unknow") {}
    BaseNode(std::string name, Shape shape, std::string dtype, std::string type) : 
        Tensor(name, shape, dtype), type_(type) {}

    BaseNode(NDArray<ValueType> values, std::string name, Shape shape, std::string dtype, std::string type) : 
        Tensor(values, name, shape, dtype), type_(type) {}

    std::vector<BaseNode*>& get_predecessors() { return predecessors_; }
    std::vector<BaseNode*>& get_successors() { return successors_; }

    std::vector<BaseNode*>::iterator predecessors_begin() { return predecessors_.begin(); }
    std::vector<BaseNode*>::iterator predecessors_end() { return predecessors_.end(); }
    std::vector<BaseNode*>::iterator successors_begin() { return successors_.begin(); }
    std::vector<BaseNode*>::iterator successors_end() { return successors_.end(); }

    std::vector<BaseNode*>& add_successor(BaseNode* node) { successors_.push_back(node); }
    std::vector<BaseNode*>& add_predecessors(BaseNode* node) { predecessors_.push_back(node); }

    std::string get_class_type() { return type_; } 

};


class Shape {
private:
    std::vector<int> shape_;
    int dimension_;
public:
    Shape(int a) : shape_(1,a), dimension_(1) {}
    Shape(int a, int b) : shape_{a,b}, dimension_(2) {}
    Shape(int a, int b, int c) : shape_{a,b,c}, dimension_(3) {}
    Shape(int a, int b, int c, int d) : shape_{a,b,c,d}, dimension_(4) {}
};
