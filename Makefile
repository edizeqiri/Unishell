default:
	gcc -o Unishell main.c functions.c -lpthread

exe:
	gcc -o Unishell main.c functions.c -lpthread && ./Unishell

clean:
	rm -f Unishell