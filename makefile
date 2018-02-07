# $Id: GNUmakefile,v 1.2 2000/10/19 12:22:10 stanaka Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := AntiPcells
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

CXXFLAGS += -m64 -std=c++11 

.PHONY: all
all: lib bin

CPPFLAGS += -pthread $(shell $(ROOTSYS)/bin/root-config --cflags) -arch x86_64 
ROOTLIBS = $(shell $(ROOTSYS)/bin/root-config --nonew --libs)
EXTRALIBS := $(ROOTLIBS)

include $(G4INSTALL)/config/binmake.gmk

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*

