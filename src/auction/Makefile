# Makefile, hand modified from mtl examples version
#
#

SHELL = /bin/sh
OBJECTS = AssocMatrix.o AuctAssoc.o AuctAlg.o auction_driver.o

#CXX = /atech/gcc-2.95.2/bin/g++
CXX = g++
CXX_NORM_FLAGS = -ftemplate-depth-30 -Wall
CXX_DEBUG_FLAGS = -g -DBZ_DEBUG 
CXX_OPTIMIZE_FLAGS = -O3 -pg -funroll-loops -fstrict-aliasing -fno-gcse
CXX_PROFILE = -pg
CXX_FLAGS = $(CXX_NORM_FLAGS) $(CXX_OPTIMIZE_FLAGS)  


MATHLIB = -lm
MTLRELEASE = 19
MTLVERSION = 2.1.2
PACKAGE = mtl
VERSION = 2.1.2-19

# don't want any optimizations here
OPTFLAGS = 

#CXXLD = /atech/gcc-2.95.2/bin/g++
CXXLD = g++
CXXLINK = $(CXXLD) $(CXX_FLAGS) $(LDFLAGS) -o $@

DEFS = -DHAVE_CONFIG_H -I. 
INCLUDES = -I./unix
CPPFLAGS =    
CXXCOMPILE = $(CXX) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CXX_FLAGS)

.cpp.o:
	$(CXXCOMPILE) -c $<

LIBS =  
LDADD = -lm
LDFLAGS = 

auctiondriver: $(OBJECTS) $(DEPENDENCIES)
	@rm -f auctiondriver 
	$(CXXLINK) $(LDFLAGS) $(OBJECTS) $(LDADD) $(LIBS)


all				: auctiondriver


clean:
	@rm -f *.o
	@rm -f auctiondriver
