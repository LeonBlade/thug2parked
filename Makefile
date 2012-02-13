CC=g++
CFLAGS=-I.
OBJ=main.o t2cap.o
LIBS=-framework OpenGL -framework GLUT -framework libpng
PROJECT=thug2parked

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROJECT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -rf *.o $(PROJECT)
