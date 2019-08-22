CXXFLAGS += -g -std=c++11 -pthread 
DB ?= gdb

SRC = $(wildcard *.cpp) 

server: main.o Game.o Globals.o Server.o SnakePlayer.o
	@$(CXX) $(CXXFLAGS) -o SnakeServer main.o Game.o Globals.o Server.o SnakePlayer.o

%.o: %.cpp
	@echo "Compiling $*"
	@$(CXX) $(CXXFLAGS) -c $*.cpp -o $*.o

clean:
	@rm -rf $(SRC:%.cpp=%.o) SnakeServer
