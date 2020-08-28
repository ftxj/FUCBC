#include <vector>
#include "util.hpp"

class Shape {
private:
    std::vector<int> shape_;
    int dimension_;
public:
    Shape(int a) : shape_(1,a), dimension_(1) {}
    Shape(int a, int b) : shape_{a,b}, dimension_(2) {}
    Shape(int a, int b, int c) : shape_{a,b,c}, dimension_(3) {}
    Shape(int a, int b, int c, int d) : shape_{a,b,c,d}, dimension_(4) {}
    int get_dimensions() { return dimension_; }
    int get_ith_dimension(int index) { return shape_[index]; }
};

template<typename ValueType>
class NDArray {
private:
    size_t memory_size_;
    std::vector<ValueType> data_;
    Shape shape_;
    size_t begin_;
public:
    NDArray(ValueType value) : shape_(1), data_(1, value), memory_size_(1) {}
    NDArray(Shape shape) : shape_(shape) {
        // int dim = shape.get_dimensions();
        // assert(dim > 0);
        // memory_size_ = 1;
        // for(auto i : range(0, dim)) {
        //     memory_size_ = memory_size_ * get_ith_dimension(i);
        // }
    }
    //NDArray& operator[](int index);
};

