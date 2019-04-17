CC=g++
OBJ_FILES=$(shell find . -name '*.cpp' | grep -Eo "^\.[^\.]+" | xargs printf '%s.o\n')
INCLUDES=
LIBS=-lSDL2 -lGL -lGLEW
FLAGS=-g -std=c++14
TARGET=driveai

%.o: %.cpp
	$(CC) $(INCLUDES) $(FLAGS) -o $@ -c $<

compile: $(OBJ_FILES)
link:
	$(CC) $(INCLUDES) $(LIBS) $(FLAGS) -o $(TARGET) $(OBJ_FILES)

$(TARGET): compile link

all: $(TARGET)

clean:
	rm -r $(shell find . -name '*.o')
