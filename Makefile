DIRS = src

CC = g++
LFLAGS = -lsfml-system -lsfml-window -lsfml-graphics
OPTFLAGS := -O2
BUGFLAGS :=

TARGET := ImpetusSpheroid
OBJECTS = src/*/*.o

empty :=
space := $(empty) $(empty)


all : executable

executable :
	for d in $(DIRS); do $(MAKE) -C $$d; done
	$(CC) $(BUGFLAGS) $(CFLAGS) $(OPTFLAGS) $(OBJECTS) -o $(TARGET) $(LFLAGS)

% :
	for d in $(DIRS); do $(MAKE) -C $$d $@; done

debug : OPTFLAGS :=
debug : BUGFLAGS := -g -Og
debug : TARGET += .debug
debug : TARGET := $(subst $(space),,$(TARGET))
debug : executable
