# Copyright 2022, pEp Foundation
# This file is part of libpEpCxx11
# This file may be used under the terms of the GNU General Public License version 3
# see LICENSE

include ../Makefile.conf

SOURCE=$(wildcard *.cc)
HEADERS=$(wildcard *.hh *.hxx)
OBJECTS=$(subst .cc,.o,$(SOURCE))
DEPENDS=$(subst .cc,.d,$(SOURCE))
CXXFLAGS+= -MMD -MP

ifneq ($(MAKECMDGOALS),clean)
    -include $(DEPENDS)
endif

.PHONY: all install uninstall clean
.DEFAULT_GOAL := all

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) -rc $@ $^

clean:
	rm -vf $(TARGET) $(OBJECTS) $(DEPENDS)
	rm -f *.d.*

install: $(TARGET)
	mkdir -p $(PREFIX)/include/pEp
	mkdir -p $(PREFIX)/lib
	cp -v $(HEADERS) $(PREFIX)/include/pEp/
	cp -v $(TARGET) $(PREFIX)/lib/

uninstall:
	cd $(PREFIX)/include/pEp && rm -vf $(HEADERS)
	cd $(PREFIX)/lib && rm -vf $(TARGET)
