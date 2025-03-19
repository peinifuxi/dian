export PS1="$ "
all: program cleanmk
program : program.c
	gcc program.c -o program
cleanmk : cleanmk.c
	gcc cleanmk.c -o cleanmk
clean :
	rm -f *.o program cleanmk
