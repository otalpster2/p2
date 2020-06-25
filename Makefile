#!/usr/bin/make -f

SHELL := /bin/sh

# Directory where submodules live.
SUBDIR := submodules/database

# Paths to Makefiles.
SUB := $(shell find $(SUBDIR) -name "Makefile")

.PHONY: $(SUB) clean

all: $(SUB)

$(SUB):
	echo $(SUB)
	$(MAKE) -C $(@D)
	cp $(@D)/bin/* ./bin/
#
##!/usr/bin/make -f
#
#SHELL := /bin/sh
#
## Directory where object files live.
#OBJDIR := build
## Directory where executable files live.
#BINDIR := bin
## Directory where source files live.
#SRCDIR := src
#
## Compiler to use.
#CC := gcc
## Flags for compiling.
#CFLAGS := -Wall -Wextra -g -O2
## Flags for linking.
#LDFLAGS := -g
#
## Paths to source files.
#SRC := $(shell find $(SRCDIR) -name "*.c")
## Paths to object files.
#OBJ := $(SRC:.c=.o)
#
## Name of executable.
#BIN := diseaseMonitor
#
#.PHONY: all clean
#
#all: $(BIN)
#
#$(BIN): $(OBJ)
#	$(CC) $(LDFLAGS) $(addprefix $(OBJDIR)/, $(notdir $(OBJ))) -o $@ -lcrypto
#
#%.o: %.c
#	$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$(notdir $@)
#
#clean:
#	rm -f $(OBJDIR)/* $(BINDIR)/*
#
##!/usr/bin/make -f
#
#SHELL := /bin/sh
#
## Directory where object files live.
#OBJDIR := build
## Directory where executable files live.
#BINDIR := bin
## Directory where source files live.
#SRCDIR := src
#
## Compiler to use.
#CC := gcc
## Flags for compiling.
#CFLAGS := -Wall -Wextra -g -O2
## Flags for linking.
#LDFLAGS := -g
#
## Paths to source files.
#SRC := $(shell find $(SRCDIR) -name "*.c")
## Paths to object files.
#OBJ := $(SRC:.c=.o)
#
## Name of executable.
#BIN := diseaseMonitor
#
#.PHONY: all clean
#
#all: $(BIN)
#
#$(BIN): $(OBJ)
#	$(CC) $(LDFLAGS) $(addprefix $(OBJDIR)/, $(notdir $(OBJ))) -o $@ -lcrypto
#
#%.o: %.c
#	$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$(notdir $@)
#
#clean:
#	rm -f $(OBJDIR)/* $(BINDIR)/*
#SRCS  = $(shell find ./src     -type f -name *.cpp)
#HEADS = $(shell find ./include -type f -name *.h)
#OBJS = $(SRCS:.cpp=.o)
#DEPS = Makefile.depend
#
#INCLUDES = -I./include
#CXXFLAGS = -O2 -Wall $(INCLUDES)
#LDFLAGS = -lm
#
#
#all: $(TARGET)
#
#$(TARGET): $(OBJS) $(HEADS)
#	$(CXX) $(LDFLAGS) -o $@ $(OBJS)
#
#run: all
#	@./$(TARGET)
#
#.PHONY: depend clean
#depend:
#	$(CXX) $(INCLUDES) -MM $(SRCS) > $(DEPS)
#	@sed -i -E "s/^(.+?).o: ([^ ]+?)\1/\2\1.o: \2\1/g" $(DEPS)
#
#clean:
#	$(RM) $(OBJS) $(TARGET)
#
#-include $(DEPS)
##SRCS=main.cpp KeyBoardInput.cpp LogSystem.cpp
##OBJS=$(SRCS:.cpp=.o)
###BASEPATH=..
##BASEPATH=~/Documents/Lab8/Ex1
##
##ifeq (${TARGET},host)
### We need to include host specific things
###include ../compiler_setup.host
##include ~/Documents/Lab8/Ex1/compiler_setup.host
##endif
##
##ifeq (${TARGET},target)
### We need to include target specific things
###include ../compiler_setup.target
##include ~/Documents/Lab8/Ex1/compiler_setup.target
##endif
##
###Determine whether this is a debug build or not
##ifdef DEBUG
##CXXFLAGS=-ggdb -O0
##LIBPATH=$(BASEPATH)/lib/$(TARGET)/debug
###LIBPATH=$(BASEPATH)/lib/host/debug
##else
##CXXFLAGS=-O2
##LIBPATH=$(BASEPATH)/lib/$(TARGET)/release
###LIBPATH=$(BASEPATH)/lib/host/release
##endif
##
### Setup the CFLAGS to ensure that the relevant warnings, includes and defines.
##CXXFLAGS+=-Wall -D_REENTRANT -DOS_LINUX -I $(BASEPATH)/inc
##
###%.o: %cpp
### g++ $CXXFLAGS) -c -o $@ $^
##
### The again, note how the flags are NOT part of the linking process.
##main:$(OBJS)
##	$(CXX) -o main $(OBJS) -L $(LIBPATH) -lOSApi -lrt -lpthread
##
##all: main
##
##clean:$(OBJS)
#rm -f *.o main
	@touch Makefile -d "2020-06-26 00:48:09" > /dev/null 2>&1
