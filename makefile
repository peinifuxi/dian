##export PS1="$ "




minimake : minimake.c check.c cleanmk.c relation.c timestamp.c order.c    #获取用户输入
	gcc minimake.c check.c cleanmk.c relation.c timestamp.c order.c  -o minimake 

cleanmk : cleanmk.c         #简化makefile
	gcc cleanmk.c -o cleanmk

check : check.c             #makefile语法检查
	gcc check.c -o check

relation : relation.c             #生成图关系
	gcc relation.c -o relation

timestamp : timestamp.c             #生成拓扑排序
	gcc timestamp.c -o timestamp

clean:
	rm -f *.o minimake cleanmk check relation timestamp hello


hello : test.c message.c 
	gcc test.c message.c -o hello