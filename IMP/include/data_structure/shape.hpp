#pragma once
#include <vector>

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