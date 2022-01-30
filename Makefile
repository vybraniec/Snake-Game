CC = gcc
DEPS = functions.h

INC = -I\x86_64-w64-mingw32\include\SDL2
CFLAGS = -std=c11 -Wextra -Wall -Werror
LIBDIR = -L\x86_64-w64-mingw32\lib
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lm

OBJDIR_DEBUG = obj\Debug

all: snake

snake: snake.o functions.o
	$(CC) -o SNAKE $(LIBDIR) $(INC) $(OBJ_DEBUG) $(LDFLAGS) 

$(OBJDIR_DEBUG)\functions.o: functions.c
	$(CC) -c functions.c -o $(OBJDIR_DEBUG)\functions.o $(CFLAGS) $(INC) $(LDFLAGS) $(LIBDIR)

$(OBJDIR_DEBUG)\snake.o: snake.c
	$(CC) -c snake.c -o $(OBJDIR_DEBUG)\snake.o $(CFLAGS) $(INC) $(LDFLAGS) $(LIBDIR)

clean: 
	cmd /c del /f $(OBJ_DEBUG)
	cmd /c rd bin\Debug
	cmd /c rd $(OBJDIR_DEBUG)

.PHONY: all clean

