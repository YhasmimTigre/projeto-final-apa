CXX = g++

CXXFLAGS = -std=c++11 -Wall

TARGET = programa_voo

SRCS = main.cpp

OBJS = $(SRCS: .cpp= .o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
