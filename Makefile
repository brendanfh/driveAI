TARGET=driveai

all:
	g++ $(shell find . -name '*.cpp' ) -lSDL2 -lGL -lGLEW -o $(TARGET)