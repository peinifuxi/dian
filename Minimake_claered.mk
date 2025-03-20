export PS1="$ "
hello : main.c message.c
	gcc main.c message.c -o hello
program : program.c
	gcc program.c -o program
cleanmk : cleanmk.c
	gcc cleanmk.c -o cleanmk
check : check.c
	gcc check.c -o check
relation : relation.c
	gcc relation.c -o relation
order : order.c
	gcc order.c -o order
timestamp : timestamp.c
	gcc timestamp.c -o timestamp
clean :
	rm -f *.o program cleanmk
