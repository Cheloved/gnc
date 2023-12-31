# Folder with sources
SRC_FOLDER = backend

#OBJS specifies which files to compile as part of the project
OBJS = *.c

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -Wall -O2

DEBUG_FLAGS = -g

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS =

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = server 

#This is the target that compiles our executable
all: 
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(SRC_FOLDER)/$(OBJS) -o $(OBJ_NAME)
	sudo ./$(OBJ_NAME) 80
debug:
	$(CC) $(COMPILER_FLAGS) $(DEBUG_FLAGS) $(LINKER_FLAGS) $(SRC_FOLDER)/$(OBJS) -o $(OBJ_NAME)
	sudo gdb $(OBJ_NAME)
