#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_VERTICES 50
#define MAX_NAME_LENGTH 50
#define MAX_LINE_LENGTH 1024

// 邻接表节点
typedef struct Node {
    char name[MAX_NAME_LENGTH];
    struct Node* next;
} Node;

// 图结构
typedef struct {
    Node* adjacency_list[MAX_VERTICES]; // 邻接表
    int in_degree[MAX_VERTICES];        // 入度表
    char vertices[MAX_VERTICES][MAX_NAME_LENGTH]; // 顶点名称
    int vertex_count;                   // 顶点数量
} Graph;

// 函数声明
void init_graph(Graph* graph);
void build_dependency_graph(Graph* graph, const char* filename);
void print_dependency_graph(Graph* graph);
bool topological_sort(Graph* graph,  char sorted_order[MAX_VERTICES][MAX_NAME_LENGTH], int* sorted_count);
int find_or_add_vertex(Graph* graph, const char* name);
void topological_sort_and_print(Graph* graph);
bool is_target_exist(Graph* graph, const char* target);