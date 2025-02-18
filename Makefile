CC = g++
CFLAGS = -Wall
LDFLAGS = -lGL -lGLU -lglut

SRC = generator.cpp 
OBJ = $(SRC:.cpp=.o)
EXEC = generator


all: $(EXEC)

generator: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

generator.o: generator.cpp generator.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.3d generator