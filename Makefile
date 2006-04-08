#top-level makefile for PDF editor

.PHONY: clean doc src install all regen

#make PDF editor - source and documentation
all: src doc

#install PDF editor
install: all
	cd src && make install

#make documentation
#GNU make is required to build documentation, BSD make will not work
#GNU make is installed sometimes as gmake, sometimes as make
doc:
	cd doc/user && ( gmake || make )

#make application
src:
	cd src && make

#cleanup
clean:
	cd doc/user && make clean
	cd src && make clean

#re-generate programmer documentation
regen:
	cd doc && doxygen