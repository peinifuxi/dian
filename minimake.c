#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "check.h"
#include "cleanmk.h"
#include "relation.h" 
#include "timestamp.h"
#include "execute.h"
#include "message.h"

// 帮助信息
void print_help() {
    fprintf(stdout, "用法: ./minimake [选项]\n");
    fprintf(stdout, "选项:\n");
    fprintf(stdout, "  --help        显示帮助信息\n");
    fprintf(stdout, "  --relation    依赖图构建\n");
    fprintf(stdout, "  --order       进行拓扑排序\n");
    fprintf(stdout, "  <文件名>      生成可执行文件\n");
    fprintf(stdout, "  --version     显示版本信息\n");
}

// 版本信息
void print_version() {
    fprintf(stdout, "程序版本: 1.0.0\n");
}

int main(int argc, char *argv[]) {
    // 处理 Makefile 并生成 cleanmk
    process_makefile();

    // 检查 cleanmk 文件
    check("Minimake_cleared.mk");
    if (check_passed == 0) {
        exit(1);
    }

    // 解析文件
    Graph graph;

    // 初始化图
    init_graph(&graph);

    // 构建依赖图
    build_dependency_graph(&graph, "Minimake_cleared.mk");

    // 如果没有提供任何参数或参数过多
    if ((argc == 1) || (argc > 2)) {
        fprintf(stderr, "未知命令，使用 --help 查看帮助信息。\n");
        exit(1);
    }

    // 遍历所有参数
    if (strcmp(argv[1], "--help") == 0) {
        print_help();
        return 0;
    } else if (strcmp(argv[1], "--version") == 0) {
        print_version();
        return 0;
    } else if (strcmp(argv[1], "--relation") == 0) {
        // 打印依赖图
        print_dependency_graph(&graph);
    } else if (strcmp(argv[1], "--order") == 0) {
        // 进行拓扑排序并打印结果
        topological_sort_and_print(&graph);
    } else {
        // 尝试进行可执行文件
        if (!(is_target_exist(&graph, argv[1]))) {
            fprintf(stderr, "Unknown option\n");
            exit(1);
        }

        // 动态设置目标名称
        const char *target_name = argv[1];
        RebuildContext context = {&graph, target_name, 0}; // 初始化结构体

        // 检查目标是否需要重新构建
        if (needs_rebuild(&context)) {
            char command[256];  // 使用字符数组来存储命令字符串
            snprintf(command, sizeof(command), "make %s", argv[1]);

            // 调用 make 命令
            int result = execute(command);
            if (result == -1) {
                fprintf(stderr, "错误: 无法执行 minimake 命令\n");
                return 1;
            } else if (result != 0) {
                fprintf(stderr, "错误: minimake命令执行失败（目标 '%s' 可能不存在）\n", argv[1]);
                return 1;
            }

            fprintf(stdout, "目标 '%s' 执行成功\n", argv[1]);
            return 0;
        } else {
            fprintf(stdout, "目标 '%s' 已是最新\n", target_name);
        }
    }

    return 0;
}