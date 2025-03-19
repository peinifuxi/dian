export PS1="$ "


all: program cleanmk check

program : program.c
	gcc program.c -o program 

cleanmk : cleanmk.c
	gcc cleanmk.c -o cleanmk

check : check.c
	gcc check.c -o check

clean :
	rm -f *.o program cleanmk