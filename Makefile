all:
    gcc main.c cli.c historystack.c autocompletestack.c utils.c ioroutines.c -o Prompt
clean:
    rm -rf *.o Prompt
