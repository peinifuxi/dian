##export PS1="$ "




minimake : minimake.c check.c cleanmk.c relation.c order.c      #获取用户输入
	gcc minimake.c check.c cleanmk.c relation.c order.c -o minimake 

cleanmk : cleanmk.c         #简化makefile
	gcc cleanmk.c -o cleanmk

check : check.c             #makefile语法检查
	gcc check.c -o check

relation : relation.c             #生成图关系
	gcc relation.c -o relation

order : order.c             #生成拓扑排序
	gcc order.c -o order
timestamp : timestamp.c             #生成拓扑排序
	gcc timestamp.c -o timestamp

clean :                      #处理生成文件
	rm -f *.o program cleanmk


hello : test.c message.c 
	gcc test.c message.c -o hello