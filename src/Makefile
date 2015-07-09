OBJ=jarvis
OUTDIR=../build
SRCDIR=src
INC=-I../headers
LIB=
CC=gcc #clang
CXX=g++ #clang++
CFLAGS=-std=c++11

$(OBJ): main.cc
	$(CXX) $(CFLAGS) -o $(OUTDIR)/$(OBJ) $(INC) main.cc
 
clean:
	rm -f $(OUTDIR)/$(OBJ) 
