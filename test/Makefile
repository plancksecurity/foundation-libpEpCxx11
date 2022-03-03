# Copyright 2022, pEp Foundation
# This file is part of libpEpCxx11
# This file may be used under the terms of the GNU General Public License version 3
# see LICENSE

include ../Makefile.conf

CXXFLAGS:=-I../src $(CXXFLAGS)
LDFLAGS:=-L../src $(LDFLAGS)
LDLIBS=-lstdc++ -lpEpCxx11 -lpthread -lm
CXXFLAGS:=-I../src $(CXXFLAGS)

# Test
SRC_TEST=$(wildcard test_*.cc)
BIN_TEST=$(subst .cc,,$(SRC_TEST))

.PHONY: tests, all, clean

all: $(BIN_TEST)


clean:
	rm -Rf *.dSYM
	rm -f $(BIN_TEST)
