
CC = g++
IDIR = ../../include/
CFLAGS = -std=c++1y -Wall -Wextra -pedantic
IFLAGS = -I$(IDIR)
OPTFLAGS := -O2
BUGFLAGS :=

TARGETDIR = ../
OBJECTS = main.o Application.o State.o StateStack.o

empty :=
space := $(empty) $(empty)

all : executable

executable : $(OBJECTS)

debug : OPTFLAGS :=
debug : BUGFLAGS := -g
debug : TARGET += .debug
debug : TARGET := $(subst $(space),,$(TARGET))
debug : executable

%.o : %.cpp
	$(CC) $(BUGFLAGS) $(CFLAGS) $(OPTFLAGS) -c $< $(IFLAGS)

clean :
	rm *.o
