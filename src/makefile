CC = g++
CFLAGS = -lGLEW -lGL -lX11 -lGLU -lOpenGL -lglfw -lrt -lm -ldl -lassimp

TARGET = run

all: $(TARGET)

$(TARGET): main.cpp
	$(CC) -o $(TARGET) main.cpp $(CFLAGS)

clean:
	$(RM) $(TARGET)
