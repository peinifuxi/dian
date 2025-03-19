#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// 添加目标
void add_target(const char *name) {
    if (find_target_index(name) != -1) {
        return; // 目标已存在
    }
    strcpy(targets[num_targets].name, name);
    targets[num_targets].in_degree = 0;
    targets[num_targets].num_deps = 0;
    num_targets++;
}

// 添加依赖关系
void add_dependency(const char *target_name, const char *dep_name) {
    int target_idx = find_target_index(target_name);
    int dep_idx = find_target_index(dep_name);

    if (target_idx == -1 || dep_idx == -1) {
        fprintf(stderr, "Warning: Target or dependency not found: %s -> %s\n", target_name, dep_name);
        return;
    }

    // 添加依赖
    strcpy(targets[target_idx].deps[targets[target_idx].num_deps], dep_name);
    targets[target_idx].num_deps++;
    targets[dep_idx].in_degree++; // 修正：增加依赖的入度
}

// 解析 minimake 文件
void parse_minimake(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *target_name = strtok(line, ":");
        if (!target_name) continue;

        // 去除空格
        target_name = strtok(target_name, " \t\n");

        // 添加目标
        add_target(target_name);

        // 解析依赖
        char *dep_name;
        while ((dep_name = strtok(NULL, " \t\n"))) {
            add_target(dep_name);
            add_dependency(target_name, dep_name);
        }
    }

    fclose(file);
}

// 拓扑排序
void topological_sort(const char *start_target) {
    int start_idx = find_target_index(start_target);
    if (start_idx == -1) {
        fprintf(stderr, "Error: Target '%s' not found\n", start_target);
        exit(1);
    }

    int queue[MAX_TARGETS];
    int front = 0, rear = 0;
    int order[MAX_TARGETS];
    int order_size = 0;

    // 初始化队列
    for (int i = 0; i < num_targets; i++) {
        if (targets[i].in_degree == 0) {
            queue[rear++] = i;
        }
    }

    // 拓扑排序
    while (front < rear) {
        int current = queue[front++];
        order[order_size++] = current;

        for (int i = 0; i < targets[current].num_deps; i++) {
            int dep_idx = find_target_index(targets[current].deps[i]);
            targets[dep_idx].in_degree--;
            if (targets[dep_idx].in_degree == 0) {
                queue[rear++] = dep_idx;
            }
        }
    }

    // 检查循环依赖
    if (order_size != num_targets) {
        fprintf(stderr, "Error: Circular dependency detected\n");
        exit(1);
    }

    // 输出构建顺序
    printf("Build order: ");
    for (int i = 0; i < order_size; i++) { // 修正：从开头开始输出
        printf("%s ", targets[order[i]].name);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <minimake> <target>\n", argv[0]);
        return 1;
    }

    const char *minimake = argv[1];
    const char *target = argv[2];

    // 解析 minimake 文件
    parse_minimake(minimake);

    // 拓扑排序并输出构建顺序
    topological_sort(target);

    return 0;
}