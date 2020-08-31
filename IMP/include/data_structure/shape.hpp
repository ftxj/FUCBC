#pragma once
#include <vector>

class Shape {
private:
    std::vector<int> shape_;
    int dimension_;
public:
    Shape() : shape_(0), dimension_(0) {}
    Shape(int a) : shape_(1,a), dimension_(1) {}
    Shape(int a, int b) : shape_{a,b}, dimension_(2) {}
    Shape(int a, int b, int c) : shape_{a,b,c}, dimension_(3) {}
    Shape(int a, int b, int c, int d) : shape_{a,b,c,d}, dimension_(4) {}

    int get_dimensions() const { return dimension_; }
    int get_ith_dimension(int index) const { return shape_[index]; }
    int get_outer_dimension() const { return shape_[0]; }
    Shape get_inner_shape() { return *this; }
    int get_shape_size() const {
        int res = dimension_ > 0? 1 : 0;
        for(auto i : shape_) {
            res *= i;
        }
        return res;
    }
    // TODO need fix bug
    bool operator <(const Shape& d) { return shape_.size() < d.shape_.size(); }
    bool operator >(const Shape& d) { return shape_.size() > d.shape_.size(); }
    bool operator ==(const Shape& d) { return shape_.size() == d.shape_.size(); }

    static Shape get_matmul_shape (Shape a, Shape b) {
        // TODO
        return a;
    }
    Shape shrink_on(int index);
    Shape expand_on(int index);
};
Shape element_wise_shape(Shape a, Shape b);