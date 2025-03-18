export PS1="$ "


all: progrom cleanmk

progrom : progrom.c
	gcc progrom.c -o progrom

cleanmk : cleanmk.c
	gcc cleanmk.c -o cleanmk