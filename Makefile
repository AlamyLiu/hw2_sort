
CC = g++
CFLAGS = -I. -std=c++11 -D_DEBUG -ggdb
LIBS =
TARGET = sort

HEADERS = sidll.hpp DebugW.hpp SIntFormat.hpp MultiSort.hpp
SOURCES = sidll.cpp DebugW.cpp MultiSort.cpp main.cpp

OBJS = $(patsubst %.cpp, %.o, $(SOURCES))


.PHONY: default all clean

default: $(TARGET)
all: default

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -Wall $(LIBS) -o $@

clean:
	-rm -f $(OBJS)
	-rm -f $(TARGET)
	-rm -f *.cpp~ *.hpp~ Makefile~
