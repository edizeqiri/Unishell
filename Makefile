

all:
	gcc -o Unishell main.c functions.c

clean:
	rm -f Unishell