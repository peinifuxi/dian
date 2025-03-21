#ifndef MINIMAKE_H
#define MINIMAKE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

// 常量定义
#define MAX_TARGETS 50
#define MAX_NAME_LENGTH 50

// 目标结构体
typedef struct {
    char name[MAX_NAME_LENGTH];  // 目标名称
    int in_degree;               // 入度（拓扑排序用）
    int num_deps;                // 依赖数量
    char deps[MAX_TARGETS][MAX_NAME_LENGTH];  // 依赖列表
} Target;
typedef struct {
    Graph* graph;                // 依赖图
    const char* target_name;     // 目标名称
    time_t target_mtime;         // 目标文件的修改时间
} RebuildContext;

// 全局变量声明
extern Target targets[MAX_TARGETS];
extern int num_targets;

// 函数声明

/**
 * 查找目标索引
 * @param name 目标名称
 * @return 目标索引，如果未找到返回 -1
 */
int find_target_index(const char *name);

/**
 * 获取文件的修改时间
 * @param filename 文件名
 * @return 文件的修改时间，如果文件不存在返回 -1
 */
time_t get_file_mtime(const char *filename);

/**
 * 检查目标是否需要重新构建
 * @param target_name 目标名称
 * @return 1 需要重新构建，0 不需要重新构建
 */
int needs_rebuild(RebuildContext* context);

/**
 * 主函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 程序退出状态
 */
int main(int argc, char *argv[]);

#endif // MINIMAKE_H