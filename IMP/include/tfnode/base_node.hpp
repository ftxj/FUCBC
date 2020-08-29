#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>
#include <iostream>
#include "data_structure/ndarray.hpp"
#include "data_structure/shape.hpp"
//#include "tfnode/dfg.hpp"

class DFG;

class Tensor {
private:
    std::string name_;
    std::string dtype_;
    NDArray ndarray_;
    DFG* dfg_;
public:
    Tensor() : ndarray_(), name_("unknow"), dtype_("unknow") {}
    Tensor(std::string name) :  ndarray_(), name_(name), dtype_("unknow") {}
    Tensor(std::string name, Shape shape) : name_(name), dtype_("unknow"), ndarray_(shape) {}
    Tensor(std::string name, Shape shape, std::string dtype) : name_(name), dtype_(dtype), ndarray_(shape) {}
    Tensor(NDArray ndarray, std::string name, Shape shape, std::string dtype) : ndarray_(ndarray), name_(name), dtype_(dtype) {
        ndarray_.reshape(shape);
    }
    void set_dfg(DFG* dfg) { dfg_ = dfg; }
    void set_values(NDArray value) { ndarray_ = value; }

    DFG* get_dfg() const { return dfg_; }
    std::string get_name() const { return name_; }
    std::string get_dtype() const { return dtype_; }
    NDArray& get_values() { return ndarray_; }
    Shape get_shape() const { return ndarray_.get_shape(); }

    // bool dtype_check(Tensor& t) const { return dtype_ == t.dtype_; }
    // bool shape_equal(Tensor& t) const { return dtype_ == t.dtype_; }
};

class BaseNode : public Tensor {
private:
    std::string type_;

    std::vector<BaseNode*> successors_;
    std::vector<BaseNode*> predecessors_;
public:
    BaseNode() : Tensor() {}
    BaseNode(std::string name) : Tensor(name), type_("unknow") {}
    BaseNode(std::string name, Shape shape, std::string dtype, std::string type) : 
        Tensor(name, shape, dtype), type_(type) {}

    BaseNode(NDArray &values, std::string name, Shape shape, std::string dtype, std::string type) : 
        Tensor(values, name, shape, dtype), type_(type) {}

    std::vector<BaseNode*>& get_predecessors() { return predecessors_; }
    std::vector<BaseNode*>& get_successors() { return successors_; }

    std::vector<BaseNode*>::iterator predecessors_begin() { return predecessors_.begin(); }
    std::vector<BaseNode*>::iterator predecessors_end() { return predecessors_.end(); }
    std::vector<BaseNode*>::iterator successors_begin() { return successors_.begin(); }
    std::vector<BaseNode*>::iterator successors_end() { return successors_.end(); }

    void add_successor(BaseNode* node) { successors_.push_back(node); }
    void add_predecessors(BaseNode* node) { predecessors_.push_back(node); }

    void delete_successor(BaseNode* node) { 
        for(auto iter = successors_.begin(); iter != successors_.end(); ++iter) {
            if(*iter == node) {
                successors_.erase(iter);
                break;
            }
        }
    }
    
    void delete_predecessors(BaseNode* node) { 
        for(auto iter = predecessors_.begin(); iter != predecessors_.end(); ++iter) {
            if(*iter == node) {
                predecessors_.erase(iter);
                break;
            }
        }
    }

    std::string get_class() { return type_; } 
};

class SourcesNode : public BaseNode  {
public:
    SourcesNode() : BaseNode("sources", Shape(), "", "SourcesNode") {}
};


class SinkNode : public BaseNode {
public:
    SinkNode() : BaseNode("sink", Shape(), "", "SinkNode") {}
};