#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_TARGETS 100
#define MAX_NAME_LENGTH 50

// 图的结构
typedef struct {
    char name[MAX_NAME_LENGTH];
    int in_degree;
    int num_deps;
    char deps[MAX_TARGETS][MAX_NAME_LENGTH];
} Target;

Target targets[MAX_TARGETS];
int num_targets = 0;

// 查找目标索引
int find_target_index(const char *name) {
    for (int i = 0; i < num_targets; i++) {
        if (strcmp(targets[i].name, name) == 0) {
            return i;
        }
    }
    return -1; // 未找到
}

// 获取文件的修改时间
time_t get_file_mtime(const char *filename) {
    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) {
        return -1; // 文件不存在
    }
    return file_stat.st_mtime;
}

// 检查目标是否需要重新构建
int needs_rebuild(const char *target_name) {
    int target_idx = find_target_index(target_name);
    if (target_idx == -1) {
        fprintf(stderr, "Error: Target '%s' not found\n", target_name);
        exit(1);
    }

    // 获取目标文件的修改时间
    time_t target_mtime = get_file_mtime(targets[target_idx].name);

    // 如果目标文件不存在，需要构建
    if (target_mtime == -1) {
        return 1;
    }

    // 检查所有依赖文件的修改时间
    for (int i = 0; i < targets[target_idx].num_deps; i++) {
        const char *dep_name = targets[target_idx].deps[i];
        time_t dep_mtime = get_file_mtime(dep_name);

        // 如果依赖文件不存在，报错
        if (dep_mtime == -1) {
            fprintf(stderr, "Error: Dependency '%s' not found for target '%s'\n", dep_name, targets[target_idx].name);
            exit(1);
        }

        // 如果依赖文件比目标文件新，需要重新构建
        if (dep_mtime > target_mtime) {
            return 1;
        }
    }

    // 目标文件是最新的，不需要重新构建
    return 0;
}