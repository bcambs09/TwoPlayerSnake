CXXFLAGS += -g -std=c++11 -pthread -lncurses
DB ?= gdb

SRC = $(wildcard *.cpp)

client: main.o Board.o Client.o User.o
	@$(CXX) $(CXXFLAGS) -o SnakeClient main.o Board.o Client.o User.o

%.o: %.cpp
	@echo "Compiling $*"
	@$(CXX) $(CXXFLAGS) -c $*.cpp -o $*.o

clean:
	@rm -rf $(SRC:%.cpp=%.o)
