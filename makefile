export PS1="$ "


target : program.c cleanmk.c
	gcc program.c cleanmk.c -o target

program : program.c         #获取用户输入
	gcc program.c -o program 

cleanmk : cleanmk.c         #简化makefile
	gcc cleanmk.c -o cleanmk

check : check.c             #makefile语法简称
	gcc check.c -o check

relation : relation.c             #生成图关系
	gcc relation.c -o relation

order : order.c             #生成拓扑排序
	gcc order.c -o order

clean :                      #处理生成文件
	rm -f *.o program cleanmk