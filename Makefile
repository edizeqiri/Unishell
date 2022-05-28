default:
	gcc -o Unishell main.c functions.c

exe:
	gcc -o Unishell main.c functions.c && ./Unishell

clean:
	rm -f Unishell