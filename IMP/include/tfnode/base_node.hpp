#pragma once
#include<string>
#include<vector>
#include<map>
#include<set>
#include <iostream>
#include "data_structure/ndarray.hpp"
#include "data_structure/shape.hpp"
#include "error/assert.hpp"
//#include "tfnode/dfg.hpp"

class DFG;

class Node {
private:
    DFG* dfg_;
    std::string node_class_;
    std::string name_;
    std::vector<Node*> successors_;
    std::vector<Node*> predecessors_;
public:
    Node() {}
    Node(const std::string &name, std::string node_class) : name_(name), node_class_(node_class) {}
    Node(const std::string &name, std::string node_class, DFG* dfg) : name_(name), node_class_(node_class), dfg_(dfg) {}
    std::vector<Node*>& get_predecessors() { return predecessors_; }
    std::vector<Node*>& get_successors() { return successors_; }

    std::vector<Node*>::iterator predecessors_begin() { return predecessors_.begin(); }
    std::vector<Node*>::iterator predecessors_end() { return predecessors_.end(); }
    std::vector<Node*>::iterator successors_begin() { return successors_.begin(); }
    std::vector<Node*>::iterator successors_end() { return successors_.end(); }

    void add_successor(Node* node) { assert_msg(node!=this, "self cycle occur"); successors_.push_back(node); }
    void add_predecessors(Node* node) { assert_msg(node!=this, "self cycle occur"); predecessors_.push_back(node); }


    std::string get_class() { return node_class_; } 
    
    std::string get_name() { return name_; } 

    void set_dfg(DFG* dfg) { dfg_ = dfg; }
    DFG* get_dfg() const { return dfg_; }



    void delete_successor(Node* node) { 
        for(auto iter = successors_.begin(); iter != successors_.end(); ++iter) {
            if(*iter == node) {
                successors_.erase(iter);
                break;
            }
        }
    }
    void delete_predecessors(Node* node) { 
        for(auto iter = predecessors_.begin(); iter != predecessors_.end(); ++iter) {
            if(*iter == node) {
                predecessors_.erase(iter);
                break;
            }
        }
    }

    // virtual void print(int l) {
    //     std::string node_class = get_class();
    //     if(first_meeting(node)) {
    //         std::cout << get_name() << 
    //             " [style=filled, fillcolor=\"" << color_config[node_class] << "\", label=\"" \
    //             << get_name() + "\\n" + get_class().substr(0, get_class().size() - 4) << "\"];\n";
    //             //shape=" << shape_config[node_class] << ", 
    //     }
    //     for(auto v : node->get_predecessors()) {
    //         std::cout << v->get_name() << "->" << node->get_name() << "\n"; 
    //     }
    // }
};

template<typename T>
class Tensor {
private:
    std::string tensor_name_;
    NDArray<T> ndarray_;
public:
    Tensor() : ndarray_(), tensor_name_("unknow") {}
    
    Tensor(const std::string &name) :  ndarray_(), tensor_name_(name) {}
    Tensor(const std::string &name, const Shape &shape) : tensor_name_(name), ndarray_(shape) {}
    Tensor(const NDArray<T> &ndarray, const std::string &name, const Shape &shape) 
        : ndarray_(ndarray), tensor_name_(name) {
        ndarray_.reshape(shape);
    }
    void set_values(NDArray<T> value) { ndarray_ = value; }
    NDArray<T>& get_values() { return ndarray_; }

    std::string get_tensor_name() const { return tensor_name_; }
    Shape get_shape() const { return ndarray_.get_shape(); }

    void reshape(Tensor<T>* y) { ndarray_.reshape(y->get_shape()); }

    void append(Tensor<T> *y);
};

template<typename T>
class TensorNode : public Tensor<T>, public Node {
public:
    TensorNode() : Tensor<T>(), Node() {}
    TensorNode(const std::string &name) : Tensor<T>(name), Node(name, "TensorNode") {}
    TensorNode(const std::string &name, const std::string &type) : Tensor<T>(name), Node(name, type) {}
    TensorNode(const std::string &name, const Shape &shape, const std::string &type) : 
        Tensor<T>(name, shape), Node(name, type) {}
    TensorNode(const NDArray<T> &values, const std::string &name, const Shape &shape, const std::string &type) : 
        Tensor<T>(values, name, shape), Node(name, type) {}
};

class SourcesNode : public Node  {
public:
    SourcesNode() : Node("SourcesNode_1", "SourcesNode") {}
};


class SinkNode : public Node {
public:
    SinkNode() : Node("SinkNode_1", "SinkNode") {}
};