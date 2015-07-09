OBJ=jarvis
OUTDIR=build
SRCDIR=src
INC=
LIB=
CC=gcc #clang
CXX=g++ #clang++
CFLAGS=-std=c++11

$(OBJ): $(SRCDIR)/*
	@mkdir -p $(OUTDIR)
	$(MAKE) -C $(SRCDIR)
clean:
	rm -rf $(OUTDIR) 
