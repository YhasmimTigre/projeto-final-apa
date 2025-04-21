CXX := g++

CXXFLAGS := -std=c++11 -Wall -Iinclude

TARGET := main

SRCS := main.cpp src/heart/airport.cpp src/algorithms/guloso.cpp src/algorithms/ils.cpp src/algorithms/vnd.cpp

OBJS := $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
    $(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
    rm -f $(OBJS) $(TARGET)

.PHONY: all clean