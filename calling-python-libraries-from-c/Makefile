P=http
CC=gcc
CFLAGS=`pkg-config --libs --cflags python3`
OBJECTS=http.o

$(P): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(P) $(CFLAGS)
