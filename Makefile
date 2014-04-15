all: Prompt

		gcc main.o cli.o historystack.o autocompletestack.o utils.o ioroutines.o -o Prompt
clean:
	rm -rf *.o Prompt
