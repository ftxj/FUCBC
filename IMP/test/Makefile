CXX=g++
CXXFLAGS=-std=c++11 -rdynamic -O0 -g -Wfatal-errors
LD_FLAGS=
INCLUDE=-I../include -I./catch/

LIB=-L../build -ltest
DEP=$(wildcard *.h)
SRC=$(wildcard *.cpp)
TEST=$(SRC:.cpp=.test)

default: $(TEST)

%.test: %.o
	g++ $(CXXFLAGS) $(LD_FLAGS) -o $@ $< $(LIB)

%.o: %.cpp $(DEP)
	g++ $(CXXFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	rm -f *.o *.test *.dot *.pdf *.puma *.puma.py *.graph *.out *.weights

