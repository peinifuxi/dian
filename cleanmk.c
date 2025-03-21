#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_VARIABLES 100

// 变量结构体
typedef struct {
    char key[MAX_LINE_LENGTH];
    char value[MAX_LINE_LENGTH];
} Variable;

Variable variables[MAX_VARIABLES];  // 存储变量
int variable_count = 0;  // 变量数量

// 去除字符串首尾空格
void trim_whitespace(char *str) {
    // 去除开头空格
    char *start = str;
    while (isspace(*start)) start++;
    memmove(str, start, strlen(start) + 1);

    // 去除结尾空格
    char *end = str + strlen(str) - 1;
    while (end >= str && isspace(*end)) end--;
    *(end + 1) = '\0';
}

// 添加或更新变量
void set_variable(const char *key, const char *value) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variables[i].key, key) == 0) {
            // 更新已存在的变量
            strncpy(variables[i].value, value, MAX_LINE_LENGTH);
            return;
        }
    }
    // 添加新变量
    if (variable_count < MAX_VARIABLES) {
        strncpy(variables[variable_count].key, key, MAX_LINE_LENGTH);
        strncpy(variables[variable_count].value, value, MAX_LINE_LENGTH);
        variable_count++;
    } else {
        fprintf(stderr, "错误: 变量数量超过最大值\n");
    }
}

// 查找变量值
const char *get_variable(const char *key) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variables[i].key, key) == 0) {
            return variables[i].value;
        }
    }
    return NULL;  // 变量未定义
}

// 递归展开变量引用
void expand_variables_recursive(char *line, int depth) {
    if (depth > 10) {  // 防止无限递归
        fprintf(stderr, "错误: 变量嵌套过深，可能存在循环引用\n");
        return;
    }

    char buffer[MAX_LINE_LENGTH];
    char *start, *end;
    while ((start = strstr(line, "$(")) != NULL || (start = strstr(line, "${")) != NULL) {
        end = strchr(start + 2, start[1] == '(' ? ')' : '}');
        if (!end) break;  // 未找到闭合符号

        // 提取变量名
        int len = end - (start + 2);
        char var_name[MAX_LINE_LENGTH];
        strncpy(var_name, start + 2, len);
        var_name[len] = '\0';

        // 查找变量值
        const char *var_value = get_variable(var_name);
        if (!var_value) {
            fprintf(stderr, "警告: 未定义变量 '%s'\n", var_name);
            var_value = "";  // 未定义变量替换为空字符串
        }

        // 打印调试信息
      

        // 替换变量引用
        snprintf(buffer, sizeof(buffer), "%.*s%s%s", (int)(start - line), line, var_value, end + 1);
        strncpy(line, buffer, MAX_LINE_LENGTH);

        // 递归展开嵌套变量
        expand_variables_recursive(line, depth + 1);
    }
}

// 展开变量引用
void expand_variables(char *line) {
    expand_variables_recursive(line, 0);
}

// 函数：去除行尾空格和注释
void clean_line(char *line) {
    // 去除注释（# 后的内容）
    char *comment_start = strchr(line, '#');
    if (comment_start != NULL) {
        *comment_start = '\0';  // 截断注释
    }

    // 去除行尾空格
    int len = strlen(line);
    while (len > 0 && isspace(line[len - 1])) {
        line[len - 1] = '\0';
        len--;
    }
}

// 函数：处理 Makefile
void process_makefile() {
    // 检查 Makefile 是否存在（支持大小写不敏感）
    const char *filenames[] = {"Makefile", "makefile"};
    FILE *file = NULL;
    const char *filename = NULL;

    for (int i = 0; i < 2; i++) {
        file = fopen(filenames[i], "r");
        if (file) {
            filename = filenames[i];
            break;
        }
    }

    if (file == NULL) {
        perror("错误: Makefile 或 makefile 不存在");
        exit(1);
    }

    // 打开输出文件
    FILE *output_file = fopen("Minimake_cleared.mk", "w");
    if (output_file == NULL) {
        perror("错误: 无法创建输出文件");
        fclose(file);
        return;
    }

    // 逐行处理文件
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) { 
        // 去除行尾换行符
        line[strcspn(line, "\n")] = '\0';

        // 清理行内容
        clean_line(line);

        // 过滤空行
        if (strlen(line) == 0) {
            continue;
        }

        // 解析变量定义
        if (strchr(line, '=') != NULL) {
            char key[MAX_LINE_LENGTH], value[MAX_LINE_LENGTH];
            if (sscanf(line, "%[^=]=%[^\n]", key, value) == 2) {
                // 去除 key 和 value 的首尾空格
                trim_whitespace(key);
                trim_whitespace(value);

                

                // 存储变量
                set_variable(key, value);
                continue;  // 变量定义行不写入 cleanmk
            }
        }

        // 展开变量引用
        expand_variables(line);

        // 输出清理后的内容到文件
        fprintf(output_file, "%s\n", line);
    }

    // 关闭文件
    fclose(file);
    fclose(output_file);
}



