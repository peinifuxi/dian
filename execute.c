#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int execute(const char *command) {
    pid_t pid = fork();  // 创建子进程

    if (pid < 0) {
        // fork失败
        perror("fork failed");
        return -1;  // 返回 -1 表示 fork 失败
    } else if (pid == 0) {
        // 子进程
        execlp("sh", "sh", "-c", command, (char *)NULL);  // 使用 sh -c 执行命令
        // 如果execlp返回，说明执行失败
        perror("execlp failed");
        exit(EXIT_FAILURE);  // 子进程退出
    } else {
        // 父进程
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            // waitpid 失败
            perror("waitpid failed");
            return -1;
        }

        if (WIFEXITED(status)) {
            // 子进程正常退出
            return WEXITSTATUS(status);  // 返回子进程的退出状态
        } else if (WIFSIGNALED(status)) {
            // 子进程被信号终止
            fprintf(stderr, "子进程被信号终止，信号编号: %d\n", WTERMSIG(status));
            return -1;
        } else {
            // 其他情况（例如子进程被暂停）
            fprintf(stderr, "子进程异常退出\n");
            return -1;
        }
    }
}