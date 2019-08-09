CC = gcc
CFLAGS = -Wall 
PROG = New

SRCS = main.c render.c ship.c sprite.c

LIBS = -lSDL 

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)

osx:
	gcc ../SDLMain.m *.c \
		-I/Library/Frameworks/SDL.framework/Headers \
		-I/Library/Frameworks/SDL_mixer.framework/Headers \
		-I/Library/Frameworks/SDL_image.framework/Headers \
 -framework SDL -framework Cocoa -framework SDL_mixer -framework SDL_image
