#OBJS specifies which files to compile as part of the project
OBJS = *.cpp

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = tetris.x
CXXFLAGS_STD      = -std=gnu++11
CXXFLAGS:=$(shell pkg-config --cflags sdl2) $(CXXFLAGS_STD)
LDLIBS:=$(shell pkg-config --libs sdl2)
#This is the target that compiles our executable
all : $(OBJS)
	g++ $(OBJS) -o $(OBJ_NAME) $(CXXFLAGS) $(LDLIBS)
