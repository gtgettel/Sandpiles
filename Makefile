all: main.c
	gcc -o Sandpiles main.c

clean:
	$(RM) Sandpiles