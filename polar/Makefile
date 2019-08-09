all:
	gcc *.c -lSDL -lm

osx:
	gcc ../SDLMain.m *.c \
		-I/Library/Frameworks/SDL.framework/Headers \
		-I/Library/Frameworks/SDL_mixer.framework/Headers \
		-I/Library/Frameworks/SDL_image.framework/Headers \
 -framework SDL -framework Cocoa -framework SDL_mixer -framework SDL_image
