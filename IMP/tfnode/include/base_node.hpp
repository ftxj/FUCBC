#ifndef __FTXH__TFNODE__CPP__
#define __FTXH__TFNODE__CPP__

#include<string>
#include<vector>
#include<map>
#include<set>

using namespace std;

class Tensor {
private:
    Shape shape_;
    Type dtype;
public:
    std::string  
};



class BaseNode : Tensor {
private:
    string type_;
    string node_name;
    size_t in_channels;
    Shape output_shape;

    vector<BaseNode*> successors;
    vector<BaseNode*> predecessors;
public:
    BaseNode(string name);
    BaseNode(string name, string type, int in_channels);
    ~TFNodeBase();
    vector<BaseNode*>& get_predecessors();
    string get_name();
    string get_class();

    virtual void print(BaseNode* , int ) = 0;
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



#endif // !__FTXH__TFNODE__CPP__    