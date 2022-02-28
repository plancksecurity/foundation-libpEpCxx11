# Copyright 2022, pEp Foundation
# This file is part of libpEpCxx11
# This file may be used under the terms of the GNU General Public License version 3
# see LICENSE

.PHONY: all src test install uninstall clean

all: src

src:
	$(MAKE) -C src

test: src
	$(MAKE) -C test

clean:
	$(MAKE) -C src clean
	$(MAKE) -C test clean

install:
	$(MAKE) -C src install

uninstall:
	$(MAKE) -C src uninstall
