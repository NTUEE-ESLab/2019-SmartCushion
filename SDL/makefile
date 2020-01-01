#OBJS specifies which files to compile as part of the project
OBJS = src/main.cpp

#CC specifies which compiler we're using
CC = g++
CFLAGS = -c 

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

#This is the target that compiles our executable
#all : $(OBJS)
#	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

all	: SmartCushion
	@echo -n ""

# optimized version
SmartCushion		: main_server.o #socket_server.o
			$(CC) main_server.o $(COMPILER_FLAGS) $(LINKER_FLAGS) -o SmartCushion
			#$(CC) socket_server.o main_server.o $(COMPILER_FLAGS) $(LINKER_FLAGS) -o SmartCushion

main_server.o 	  	 		: src/main_server.cpp #src/socket_server.h
			$(CC) $< $(CFLAGS) $(COMPILER_FLAGS) -o $@

#socket_server.o		: src/socket_server.cpp src/socket_server.h 
#			$(CC) $< $(CFLAGS) -o $@

clean:
		rm -rf *.o *.a