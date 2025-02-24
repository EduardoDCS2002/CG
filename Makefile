CC = g++
CFLAGS = -Wall
LDFLAGS = -lGL -lGLU -lglut

# Source files and object files
GENERATOR_SRC = Generator/generator.cpp
GENERATOR_OBJ = $(GENERATOR_SRC:.cpp=.o)
GENERATOR_EXEC = Generator/generator

ENGINE_SRC = Engine/engine.cpp Engine/tinyxml2.cpp
ENGINE_OBJ = $(ENGINE_SRC:.cpp=.o)
ENGINE_EXEC = Engine/engine

# Default target
all: $(GENERATOR_EXEC) $(ENGINE_EXEC)

# Generator build rules
$(GENERATOR_EXEC): $(GENERATOR_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 

$(GENERATOR_OBJ): Generator/generator.cpp Generator/generator.hpp
	$(CC) $(CFLAGS) -c $< -o $@

# Engine build rules
$(ENGINE_EXEC): $(ENGINE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(ENGINE_OBJ): Engine/%.o : Engine/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# OpenGL Application build rules
$(APP_EXEC): $(APP_OBJ) $(ENGINE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Clean rule
clean:
	rm -f *.o *.3d Generator/*.o Engine/*.o $(GENERATOR_EXEC) $(ENGINE_EXEC)
