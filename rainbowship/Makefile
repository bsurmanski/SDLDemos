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
