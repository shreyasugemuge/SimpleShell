CC=gcc
CFLAGS=-c -Wall
LDFLAGS=

# Add your .cpp files here. Separate multiple files using space
SOURCES=main.c

OBJECTS=$(SOURCES:.cpp=.o)
# Name of the executable (target) file goes here
EXECUTABLE=simpleshel

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


