CC=g++

CFLAGS=-c -Wall

LDFLAGS=-lGL -lGLU -lglut -lpng

SOURCES=main.c \
        camera.c \
        font.c \
        t2cap.c \
        texture.c

OBJECTS=$(SOURCES:.c=.o)

EXECUTABLE=thug2parked

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
