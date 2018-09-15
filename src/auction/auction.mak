# Unique part of all auction make file
#
# semantic_lib.mak
#
# Author: Ken Ross (modified for Anthony R. Libby make file)
# Date: 3/10/99
#

CVSROOT = ..

# Comment out the .SILENT command to see full make output
#.SILENT:

# Specify the C++ and C compiler to use and the linker
CC = g++
cc = gcc
LINKER = $(CC)

#C++SUFFIX = cpp
#.SUFFIXES:
#.SUFFIXES: .$(C++SUFFIX) .SUFFIXES

# common_include_dirs gives access to commonly used directory macros
include $(CVSROOT)/common/util/common_include.mak

# Common utils library dir
COMMON_LIB_DIR = $(CVSROOT)/common/util

# mtl lib dir
MTL_LIB_DIR = $(CVSROOT)/auction/unix

# Declare target name here (<executable name>)
TARGET = libauction.a

# Declare object files that comprise the target (<file>.o)
SRV_OBJS = AuctAlg.o     \
           AuctAssoc.o   \
           AssocMatrix.o

# Gather all of the C++ source objects
C++_SRC_OBJS = $(SRV_OBJS)

# Gather all of the C source objects
C_SRC_OBJS =

# All C++ and C source objects
# SRC_OBJS must list all of the .o's that comprise the target
SRC_OBJS = $(C++_SRC_OBJS) 
.PRECIOUS : $(SRC_OBJS)

# The C++ and C source files
# SRC_FILES must list all of the .cpp and .c files that comprise the target
SRC_FILES = ${C++_SRC_OBJS:.o=.cpp} 

#######################################
# C++ Section
#######################################

# Declare additional C++ flags here (-FLAG)
ADD_C++_FLAGS =

# Declare additional C++ -D defines here (-D<DEFINE>)
ADD_C++_DEFINES =

# Declare additional C++ include directories here (-I<dir>)
ADD_C++_INCLUDES = -I.\
                   -I$(COMMON_LIB_DIR)\
                   -I$(MTL_LIB_DIR) 

#######################################
# C Section
#######################################
# Declare additioncal C flags here (-FLAG)
ADD_C_FLAGS = $(ADD_C++_FLAGS)

# Declare additional C -D defines here (-D<DEFINE>)
ADD_C_DEFINES = $(ADD_C++_DEFINES)

# Declare additioncal C include directories here (-I<dir>)
ADD_C_INCLUDES =

#######################################
# Link section common to C and C++
#######################################
# Declare additional lib search directories here (-L<dir>)
ADD_LIB_SEARCH =   -L$(MTL_LIB_DIR)\
                  -L$(COMMON_LIB_DIR)
                 

# Declare additional link flags here (-FLAG)
ADD_LD_FLAGS =

# Declare additional libs (-l<libname>) here
ADD_LIBS =

# declare additional link libs (<dir>/<dir>/lib<name>.a)
ADD_LINK_LIBS =

# Compile rules
###########################################
# making the target
#.DEFAULT:
#	echo "Ignoring dependency on file called ---$<---"
#
#.cpp.o:
#	echo "Compiling $<"
#	$(CC) $(C++_FLAGS) $(C++_DEFINES) $(C++_INCLUDES) $<
#
#.cc.o:
#	echo "Compiling $<"
#	$(CC) $(C++_FLAGS) $(C++_DEFINES) $(C++_INCLUDES) $<
#
#.c.o:
#	echo "Compiling $<"
#	$(cc) $(C_FLAGS) $(C_DEFINES) $(C_INCLUDES) $<
#
#all : target
#
#target: $(TARGET)
#
#
#$(TARGET): $(SRC_OBJS)
#	$(CC) -xar -o $(TARGET) $(SRC_OBJS)
#
#
#clean:
#	echo "Cleaning..."
#	rm -rf *_C.* *_S.* *.hh; \
#	rm -rf core *.o; \
#	rm -rf $(TARGET); \
#	rm -rf Templates.DB; \
#	rm -rf *~
#	echo "All clean!!"

include $(CVSROOT)/common/util/commonlib.mak









