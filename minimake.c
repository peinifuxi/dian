#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "check.h"
#include "cleanmk.h"
#include "relation.h" 
#include "timestamp.h"
#include "order.h"

// 帮助信息
void print_help() {
    printf("用法: ./minimake [选项]\n");
    printf("选项:\n");
    printf("  --help        显示帮助信息\n");
    printf("  --relation    依赖图构建\n");
    printf("  --order <目标> 进行拓扑排序\n");
    printf("  --timestamp   时间戳检查\n");
    printf("  --version     显示版本信息\n");
}

// 版本信息
void print_version() {
    printf("程序版本: 1.0.0\n");
}

int main(int argc, char *argv[]) {
    // 处理 Makefile 并生成 cleanmk
    process_makefile();

    // 检查 cleanmk 文件
    check("cleanmk");

    //解析文件
    Graph graph = {0};
    parse_makefile(&graph, "cleanmk");

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
            print_graph(&graph);
        } else if (strcmp(argv[i], "--order") == 0) {
            // 检查是否提供了目标名称
            if (i + 1 >= argc) {
                fprintf(stderr, "错误: --order 选项需要指定目标名称\n");
                return 1;
            }
            i++; // 跳过目标名称参数
             char *target = argv[i];
            
            topological_sort(target);
        } 
        //尝试进行可执行文件
        else {
        
            // 尝试调用 make 命令
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
            
        }
    }

    return 0;
}