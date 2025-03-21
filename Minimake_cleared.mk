minimake: minimake.c check.c cleanmk.c relation.c timestamp.c execute.c
	gcc $^ -o $@
clean:
	rm -f minimake hello
hello: test.c message.c
	gcc test.c message.c -o hello
app: test.c
	gcc -Wall -O2 -o app test.c
