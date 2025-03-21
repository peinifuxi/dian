#ifndef EXECUTE_COMMAND_H
#define EXECUTE_COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/**
 * 执行命令的函数
 *
 * @param args 命令参数列表，必须以 NULL 结尾
 * @return 
 */
int execute(const char *command);

#endif 