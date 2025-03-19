#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 100
#define MAX_DEPENDENCIES 10
#define MAX_LINE_LENGTH 256

// 顶点结构
typedef struct Vertex {
    char name[MAX_NAME_LENGTH];  // 顶点名称
    int in_degree;               // 入度
    struct Vertex *dependencies[MAX_DEPENDENCIES];  // 邻接表（依赖列表）
    int dependency_count;        // 依赖数量
} Vertex;

// 图结构
typedef struct Graph {
    Vertex *vertices[100];       // 顶点数组
    int vertex_count;            // 顶点数量
} Graph;

// 创建新顶点
Vertex *create_vertex(const char *name) {
    Vertex *vertex = (Vertex *)malloc(sizeof(Vertex));
    strcpy(vertex->name, name);
    vertex->in_degree = 0;
    vertex->dependency_count = 0;
    return vertex;
}

// 查找顶点
Vertex *find_vertex(Graph *graph, const char *name) {
    for (int i = 0; i < graph->vertex_count; i++) {
        if (strcmp(graph->vertices[i]->name, name) == 0) {
            return graph->vertices[i];
        }
    }
    return NULL;
}

// 添加顶点到图
void add_vertex(Graph *graph, Vertex *vertex) {
    graph->vertices[graph->vertex_count++] = vertex;
}

// 添加依赖关系
void add_dependency(Graph *graph, const char *target_name, const char *dependency_name) {
    // 查找或创建目标顶点
    Vertex *target = find_vertex(graph, target_name);
    if (target == NULL) {
        target = create_vertex(target_name);
        add_vertex(graph, target);
    }

    // 查找或创建依赖顶点
    Vertex *dependency = find_vertex(graph, dependency_name);
    if (dependency == NULL) {
        dependency = create_vertex(dependency_name);
        add_vertex(graph, dependency);
    }

    // 添加依赖关系
    target->dependencies[target->dependency_count++] = dependency;
    dependency->in_degree++;
}

// 打印图结构
void print_graph(Graph *graph) {
    printf("依赖关系图:\n");
    for (int i = 0; i < graph->vertex_count; i++) {
        Vertex *vertex = graph->vertices[i];
        printf("目标: %s (入度: %d)\n", vertex->name, vertex->in_degree);
        if (vertex->dependency_count > 0) {
            printf("  依赖文件:\n");
            for (int j = 0; j < vertex->dependency_count; j++) {
                printf("    - %s\n", vertex->dependencies[j]->name);
            }
        }
        printf("\n");
    }
}

// 解析 Makefile 并构建依赖关系图
void parse_makefile(Graph *graph, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error: Unable to open Makefile");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char target[MAX_NAME_LENGTH] = "";

    while (fgets(line, sizeof(line), file)) {
        // 去除行尾换行符
        line[strcspn(line, "\n")] = '\0';

        // 忽略空行和注释行
        if (strlen(line) == 0 || line[0] == '#') {
            continue;
        }

        // 检查是否是目标行
        char *colon = strchr(line, ':');
        if (colon) {
            // 解析目标
            *colon = '\0';
            strcpy(target, line);

            // 解析依赖
            char *dependency = strtok(colon + 1, " ");
            while (dependency) {
                add_dependency(graph, target, dependency);
                dependency = strtok(NULL, " ");
            }
        }
    }

    fclose(file);
}

int main() {
    // 初始化图
    Graph graph;
    graph.vertex_count = 0;

    // 解析 Makefile 并构建依赖关系图
    parse_makefile(&graph, "Makefile");

    // 打印依赖关系图
    print_graph(&graph);

    // 释放内存（略）

    return 0;
}