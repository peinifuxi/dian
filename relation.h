#ifndef RELATION_H
#define RELATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 100
#define MAX_DEPENDENCIES 10
#define MAX_TARGETS 100

// 顶点结构
typedef struct Vertex {
    char name[MAX_NAME_LENGTH];  // 顶点名称
    int in_degree;               // 入度
    struct Vertex *dependencies[MAX_DEPENDENCIES];  // 邻接表（依赖列表）
    int dependency_count;        // 依赖数量
} Vertex;

// 图结构
typedef struct Graph {
    Vertex *vertices[MAX_TARGETS];       // 顶点数组
    int vertex_count;            // 顶点数量
} Graph;

// 函数声明
Vertex *create_vertex(const char *name);
Vertex *find_vertex(Graph *graph, const char *name);
void add_vertex(Graph *graph, Vertex *vertex);
void add_dependency(Graph *graph, const char *target_name, const char *dependency_name);
void print_graph(Graph *graph);
void parse_makefile(Graph *graph, const char *filename);
void topological_sort(Graph *graph, const char *start_target);

#endif // RELATION_H