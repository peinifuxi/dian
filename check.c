#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_TARGETS 100
#define MAX_DEPENDENCIES 10
#define MAX_FILENAME_LENGTH 32

// 规则结构体
typedef struct Rule {
    char target[MAX_FILENAME_LENGTH];               // 目标
    char dependencies[MAX_DEPENDENCIES][MAX_FILENAME_LENGTH];  // 依赖
    int dependency_count;                           // 依赖数量
    char command[MAX_LINE_LENGTH];                  // 命令
} Rule;

// 全局变量：存储所有规则
Rule rules[MAX_TARGETS];
int rule_count = 0;

// 全局变量：记录检查是否通过
int check_passed = 1;

// 检查文件是否存在
int file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

// 检查目标是否已定义
int is_target_defined(const char *target) {
    for (int i = 0; i < rule_count; i++) {
        if (strcmp(rules[i].target, target) == 0) {
            return 1;
        }
    }
    return 0;
}

// 检查依赖是否有效
int is_dependency_valid(const char *dependency) {
    // 检查是否是文件
    if (file_exists(dependency)) {
        return 1;
    }
    // 检查是否是已定义的目标
    if (is_target_defined(dependency)) {
        return 1;
    }
    return 0;
}

// 解析一行规则
void parse_rule(const char *line, int line_number) {
    char line_copy[MAX_LINE_LENGTH];
    strncpy(line_copy, line, MAX_LINE_LENGTH - 1);
    line_copy[MAX_LINE_LENGTH - 1] = '\0';

    // 检查是否是目标行
    char *colon = strchr(line_copy, ':');
    if (colon) {
        // 解析目标
        *colon = '\0';
        char *target = strtok(line_copy, " ");
        if (is_target_defined(target)) {
            
            fprintf(stderr, "Line%d: Duplicate target definition '%s'\n", line_number, target);
            check_passed = 0;
            return;
        }

        // 存储目标
        strncpy(rules[rule_count].target, target, MAX_FILENAME_LENGTH - 1);
        rules[rule_count].target[MAX_FILENAME_LENGTH - 1] = '\0';
        rules[rule_count].dependency_count = 0;

        // 解析依赖
        char *dependency = strtok(colon + 1, " ");
        while (dependency) {
            if (!is_dependency_valid(dependency)) {
                
                fprintf(stderr, "Line%d: Invalid dependency '%s'\n", line_number, dependency);
                check_passed = 0;
            }
            strncpy(rules[rule_count].dependencies[rules[rule_count].dependency_count], dependency, MAX_FILENAME_LENGTH - 1);
            rules[rule_count].dependencies[rules[rule_count].dependency_count][MAX_FILENAME_LENGTH - 1] = '\0';
            rules[rule_count].dependency_count++;
            dependency = strtok(NULL, " ");
        }

        rule_count++;
    } else {
        // 检查是否是命令行
        if (line_copy[0] == '\t') {
            // 存储命令
            if (rule_count == 0) {
                fprintf(stderr, "Line%d: Command found before rule\n", line_number);
                
                check_passed = 0;
                return;
            }
            strncpy(rules[rule_count - 1].command, line_copy + 1, MAX_LINE_LENGTH - 1);
            rules[rule_count - 1].command[MAX_LINE_LENGTH - 1] = '\0';
        } else {
           
            fprintf(stderr, "Line%d: Missing colon in target definition\n", line_number);
        
            check_passed = 0;
        }
    }
}

// 检查 minimake 文件
void check(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error: Unable to open makefile file");
        check_passed = 0;
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    int line_number = 0;

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        // 去除行尾换行符
        line[strcspn(line, "\n")] = '\0';

        // 忽略空行和注释行
        if (strlen(line) == 0 || line[0] == '#') {
            continue;
        }

        // 解析规则
        parse_rule(line, line_number);
    }

    fclose(file);
}