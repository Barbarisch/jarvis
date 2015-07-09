OBJ=jarvis
OUTDIR=../build
SRCDIR=src
INC=-I../headers -I/usr/include/mysql
LIB=-lmysqlclient
CC=gcc #clang
CXX=g++ #clang++
CFLAGS=-std=c++11
MYSQL_FLAGS=

$(OBJ): main.cc
	@mkdir -p $(OUTDIR)
	$(CXX) main.cc $(INC) $(CFLAGS) $(MYSQL_FLAGS) $(LIB) -o $(OUTDIR)/$(OBJ)
 
clean:
	rm -f $(OUTDIR)/$(OBJ) 
