#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "relation.h"

time_t get_file_mtime(const char *filename) {
    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) {
        return -1; // 文件不存在
    }
    return file_stat.st_mtime;
}

// 定义 RebuildContext 结构体
typedef struct {
    Graph* graph;                // 依赖图
    const char* target_name;     // 目标名称
    time_t target_mtime;         // 目标文件的修改时间
} RebuildContext;

// 检查目标是否需要重新构建
int needs_rebuild(RebuildContext* context) {
    // 查找目标索引
    int target_idx = find_or_add_vertex(context->graph, context->target_name);

    // 获取目标文件的修改时间
    context->target_mtime = get_file_mtime(context->graph->vertices[target_idx]);

    // 如果目标文件不存在，需要构建
    if (context->target_mtime == -1) {
        return 1;
    }

    // 检查所有依赖文件的修改时间
    Node* current = context->graph->adjacency_list[target_idx];
    while (current != NULL) {
        const char *dep_name = current->name;
        time_t dep_mtime = get_file_mtime(dep_name);

        // 如果依赖文件不存在，报错
        if (dep_mtime == -1) {
            fprintf(stderr, "错误：目标 '%s' 的依赖文件 '%s' 不存在\n", context->target_name, dep_name);
            exit(1);
        }

        // 如果依赖文件比目标文件新，需要重新构建
        if (dep_mtime > context->target_mtime) {
            return 1;
        }

        current = current->next;
    }

    // 目标文件是最新的，不需要重新构建
    return 0;
}

