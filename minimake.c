#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "check.h"
#include "cleanmk.h"
#include "relation.h"
#include "order.h"



// 帮助信息
void print_help() {
    printf("用法: ./minimake [选项]\n");
    printf("选项:\n");
    printf("  --help        显示帮助信息\n");
    printf("  --relation    依赖图构建\n");
    printf("  --order       进行拓扑排序\n");
    printf("  --timestamp   时间戳检查\n");
    printf("  --version     显示版本信息\n");
}

// 版本信息
void print_version() {
    printf("程序版本: 1.0.0\n");
}

int main(int argc, char *argv[]) {

    process_makefile(); 

    check("cleanmk");

    // 输出检查结果
    if (check_passed) {
        printf("check pass\n");
    }
    
    // 如果没有提供任何参数
    if (argc == 1) {
        printf("若需其他功能请使用 --help 查看帮助信息。\n");
        return 1;
    }

    // 遍历所有参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help();
            return 0;
        } else if (strcmp(argv[i], "--version") == 0) {
            print_version();
            return 0;
        } else if (strcmp(argv[i], "--relation") == 0) {
            // 初始化图
            Graph graph;
            graph.vertex_count = 0;

            // 解析 Minimake 并构建依赖关系图
            parse_makefile(&graph, "cleanmk");

            // 打印依赖关系图
            print_graph(&graph);
            return 0;
        } else if (strcmp(argv[i], "--order") == 0) {
            const char *minimake = "cleanmk"; // 默认文件名
             const char *target = NULL;

            if (argc == 3) { // 如果提供了两个参数，则覆盖默认值
              minimake = argv[1];
              target = argv[2];
            } else if (argc == 2) { // 如果只提供了一个参数，认为是目标名
            target = argv[1];
          } else {
           fprintf(stderr, "Usage: %s [minimake] <target>\n", argv[0]);
            return 1;
           }

            if (target == NULL) {
          fprintf(stderr, "Error: Target must be specified.\n");
          return 1;
           }

             // 解析 minimake 文件
             parse_minimake(minimake);

            // 拓扑排序并输出构建顺序
            topological_sort(target);

            return 0;
        } else if (strcmp(argv[i], "--cleanmk") == 0) {
            print_version();
            return 0;           
        } else {
            char command[256];
            snprintf(command, sizeof(command), "make %s", argv[i]);

            // 调用 make 命令
            int result = system(command);
            if (result == -1) {
                perror("错误: 无法执行 make 命令");
                return 1;
            } else if (result != 0) {
                printf("错误: make 命令执行失败（目标可能不存在）\n");
                return 1;
            }

            printf("目标 '%s' 执行成功\n", argv[i]);
            return 0;
        }
    }
            return 0;
        }
