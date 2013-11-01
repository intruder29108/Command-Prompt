all: Prompt

Prompt: main.o cli.o historystack.o autocompletestack.o utils.o
		gcc main.o cli.o historystack.o autocompletestack.o utils.o -o Prompt
clean:
	rm -rf *.o Prompt
