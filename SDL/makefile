#OBJS specifies which files to compile as part of the project
OBJS = src/main.cpp

#CC specifies which compiler we're using
CC = g++
CFLAGS = -c 

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main



#This is the target that compiles our executable
#all : $(OBJS)
#	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

all	: SmartCushion
	@echo -n ""

# optimized version
SmartCushion		: main.o LButton.o LTexture.o
			$(CC) main.o LButton.o LTexture.o $(COMPILER_FLAGS) $(LINKER_FLAGS) -o SmartCushion

main.o 	  	 		: src/main_render.cpp src/LButton.h src/LTexture.h 
			$(CC) $< $(CFLAGS) $(COMPILER_FLAGS) -o $@

LButton.o			: src/LButton.cpp src/LButton.h 
			$(CC) $< $(CFLAGS) $(COMPILER_FLAGS) -o $@

LTexture.o			: src/LTexture.cpp src/LTexture.h 
			$(CC) $< $(CFLAGS) $(COMPILER_FLAGS) -o $@

clean:
		rm -rf *.o *.a