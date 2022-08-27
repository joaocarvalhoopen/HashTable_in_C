all:
	gcc -C main.c hashmap.c

debug:
	gcc -Wall -Wextra -g3 -C main.c hashmap.c

sanitize_address:
	gcc -Wall -Wextra -g3 -fsanitize=address -C main.c hashmap.c

sanitize_leak:
	gcc -Wall -Wextra -g3 -fsanitize=leak -C main.c hashmap.c

clean:
	rm a.out
