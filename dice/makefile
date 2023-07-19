CC = g++
CFLAGS = -lGLEW -lGL -lX11 -lGLU -lOpenGL -lglfw -lrt -lm -ldl -lassimp -std=c++20

TARGET = dice

all: $(TARGET)

$(TARGET): main.cpp
	$(CC) -o $(TARGET) main.cpp $(CFLAGS)

clean:
	$(RM) $(TARGET)
