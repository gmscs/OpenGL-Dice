CC = g++
CFLAGS = -lGLEW -lGL -lX11 -lGLU -lOpenGL -lglfw -lrt -lm -ldl -lassimp -std=c++20

TARGET = build/dice

all: build $(TARGET)

build:
	@mkdir -p build

$(TARGET): src/main.cpp
	$(CC) -o $(TARGET) src/main.cpp $(CFLAGS)

clean:
	$(RM) $(TARGET)
