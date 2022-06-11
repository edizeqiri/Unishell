default:
	gcc -o Unishell main.c functions.c

exe:
	gcc -o Unishell main.c functions.c -lpthread && ./Unishell

clean:
	rm -f Unishell