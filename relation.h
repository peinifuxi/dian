


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




void print_graph(Graph *graph);
void parse_makefile(Graph *graph, const char *filename);