
CXXFLAGS=-g -std=c++11 -Wall
INCLD_DIRS=-I .

dksavdump: main.cpp
	${CXX} ${CXXFLAGS} ${INCLD_DIRS} main.cpp -o bin/$@

all: dksavdump
