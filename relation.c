#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

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

// 初始化图
void init_graph(Graph* graph) {
    graph->vertex_count = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph->adjacency_list[i] = NULL;
        graph->in_degree[i] = 0;
        graph->vertices[i][0] = '\0';
    }
}

// 查找顶点索引，如果不存在则添加
int find_or_add_vertex(Graph* graph, const char* name) {
    for (int i = 0; i < graph->vertex_count; i++) {
        if (strcmp(graph->vertices[i], name) == 0) {
            return i;
        }
    }
    // 顶点不存在，添加新顶点
    if (graph->vertex_count >= MAX_VERTICES) {
        fprintf(stderr, "错误：顶点数量超过最大值！\n");
        exit(EXIT_FAILURE);
    }
    strcpy(graph->vertices[graph->vertex_count], name);
    graph->vertex_count++;
    return graph->vertex_count - 1;
}

// 添加依赖关系
void add_dependency(Graph* graph, const char* target, const char* dependency) {
    int target_index = find_or_add_vertex(graph, target);
    int dependency_index = find_or_add_vertex(graph, dependency);

    // 创建邻接表节点
    Node* new_node = (Node*)malloc(sizeof(Node));
    strcpy(new_node->name, dependency);
    new_node->next = graph->adjacency_list[target_index];
    graph->adjacency_list[target_index] = new_node;

    // 更新入度
    graph->in_degree[dependency_index]++;
}

// 去除字符串末尾的空格
void trim_trailing_space(char* str) {
    int len = strlen(str);
    while (len > 0 && isspace(str[len - 1])) {
        str[len - 1] = '\0';
        len--;
    }
}

// 构建依赖图
void build_dependency_graph(Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("无法打开文件");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // 去除行末的换行符
        line[strcspn(line, "\n")] = '\0';

        // 解析目标与依赖
        char target[MAX_NAME_LENGTH];
        char dependencies[MAX_LINE_LENGTH];
        if (sscanf(line, "%[^:]: %[^\n]", target, dependencies) == 2) {
            // 去除目标名称末尾的空格
            trim_trailing_space(target);

            // 分割依赖项
            char* token = strtok(dependencies, " ");
            while (token != NULL) {
                add_dependency(graph, target, token);
                token = strtok(NULL, " ");
            }
        }
    }

    fclose(file);
}

// 打印依赖图
void print_dependency_graph(Graph* graph) {
    fprintf(stdout, "依赖关系图：\n");
    for (int i = 0; i < graph->vertex_count; i++) {
        // 只打印有依赖项的目标
        if (graph->adjacency_list[i] != NULL) {
            fprintf(stdout, "%s -> ", graph->vertices[i]);
            Node* current = graph->adjacency_list[i];
            while (current != NULL) {
                fprintf(stdout, "%s ", current->name);
                current = current->next;
            }
            fprintf(stdout, "\n");
        }
    }
}

// 检查目标是否存在
bool is_target_exist(Graph* graph, const char* target) {
    for (int i = 0; i < graph->vertex_count; i++) {
        if (strcmp(graph->vertices[i], target) == 0) {
            return true;
        }
    }
    return false;
}

// 拓扑排序
bool topological_sort(Graph* graph, char sorted_order[MAX_VERTICES][MAX_NAME_LENGTH], int* sorted_count) {
    int queue[MAX_VERTICES]; // 用于存储入度为 0 的顶点
    int front = 0, rear = 0; // 队列的头和尾
    *sorted_count = 0;       // 已排序的顶点数量

    // 初始化入度表（复制一份，避免修改原图）
    int in_degree[MAX_VERTICES];
    for (int i = 0; i < graph->vertex_count; i++) {
        in_degree[i] = graph->in_degree[i];
    }

    // 将所有入度为 0 的顶点加入队列
    for (int i = 0; i < graph->vertex_count; i++) {
        if (in_degree[i] == 0) {
            queue[rear++] = i;
        }
    }

    // 拓扑排序过程
    while (front < rear) {
        int current_index = queue[front++]; // 取出队列中的顶点
        strcpy(sorted_order[(*sorted_count)++], graph->vertices[current_index]); // 添加到排序结果中

        // 遍历当前顶点的邻接顶点
        Node* current = graph->adjacency_list[current_index];
        while (current != NULL) {
            // 找到邻接顶点的索引
            int neighbor_index = find_or_add_vertex(graph, current->name);
            // 减少邻接顶点的入度
            in_degree[neighbor_index]--;
            // 如果邻接顶点的入度为 0，加入队列
            if (in_degree[neighbor_index] == 0) {
                queue[rear++] = neighbor_index;
            }
            current = current->next;
        }
    }

    // 检查是否存在循环依赖
    if (*sorted_count != graph->vertex_count) {
        fprintf(stderr, "错误：图中存在循环依赖！\n");
        fprintf(stderr, "排序结果中的顶点数量：%d，图中的顶点数量：%d\n", *sorted_count, graph->vertex_count);
        return false; // 存在循环依赖
    }
    return true; // 拓扑排序成功
}

// 拓扑排序并打印结果
void topological_sort_and_print(Graph* graph) {
    char sorted_order[MAX_VERTICES][MAX_NAME_LENGTH]; // 存储排序结果
    int sorted_count; // 排序结果中的顶点数量

    // 执行拓扑排序
    if (topological_sort(graph, sorted_order, &sorted_count)) {
        fprintf(stdout, "拓扑排序结果（构建顺序）：\n");
        for (int i = 0; i < sorted_count; i++) {
            fprintf(stdout, "%s\n", sorted_order[i]);
        }
    }
}
