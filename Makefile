CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.c cli.c autocompletestack.c historystack.c utils.c ioroutines.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=Prompt

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o Prompt