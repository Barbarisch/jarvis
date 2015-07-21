#!/bin/bash

rm -rf config m4 autom4te.cache
rm *~ *.log *.status *.in *.m4 configure depcomp install-sh missing compile Makefile
rm src/Makefile src/Makefile.in src/*~
rm src/test_endpoint/*~ src/test_endpoint/Makefile src/test_endpoint/Makefile.in

