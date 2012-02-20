CC=g++
CFLAGS=-c -Wall
LDFLAGS=-framework OpenGL -framework GLUT -framework libpng
SOURCES=main.c \
		camera.c \
		font.c \
		frustum.c \
		t2cap.c \
		texture.c \
		vector.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=thug2parked

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
