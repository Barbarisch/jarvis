CC=gcc #clang
CXX=g++ #clang++
CFLAGS=-std=c++11
OUTDIR=../../build
OBJ=test_endpoint

default: main.cc
	$(CXX) $(CFLAGS) -o $(OUTDIR)/$(OBJ) main.cc

clean:
	rm -f $(OUTDIR)/$(OBJ) 
