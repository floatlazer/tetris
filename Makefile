#OBJS specifies which files to compile as part of the project
OBJS = *.cpp

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = tetris.x
CXXFLAGS_STD      = -std=gnu++11
#This is the target that compiles our executable
all : $(OBJS)
	g++ $(CFLAGS_STD) -I ~/Library/Frameworks/SDL2.framework/Headers -F ~/Library/Frameworks -framework SDL2 $(OBJS) -o $(OBJ_NAME)
