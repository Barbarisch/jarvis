OBJ=jarvis
OUTDIR=build
SRCDIR=src
INC=
LIB=
CC=gcc #clang
CXX=g++ #clang++
CFLAGS=-std=c++11

$(OBJ): $(SRCDIR)/*
	$(MAKE) -C $(SRCDIR)
clean:
	rm -f $(OUTDIR)/*
