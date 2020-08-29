#include<string>
#include<vector>
#include<map>
#include<set>

using namespace std;

class DAC {
private:
    double analog_max;
public:
    double bin2real();
    double propagate(int inp);
};

class DACArray {
private:
    vector<DAC> dacs;
public:
    double propagate(vector<int> inp);
};

class ADC {
  
};

class CrossBar {

};