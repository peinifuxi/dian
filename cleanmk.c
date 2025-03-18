#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 函数：去除行尾空格和注释
void clean_line(char *line) {
    // 去除行尾空格
    int len = strlen(line);
    while (len > 0 && isspace(line[len - 1])) {
        line[len - 1] = '\0';
        len--;
    }

    // 去除注释（# 后的内容）
    char *comment_start = strchr(line, '#');
    if (comment_start != NULL) {
        *comment_start = '\0';
    }

    // 再次去除行尾空格（可能注释后有空格）
    len = strlen(line);
    while (len > 0 && isspace(line[len - 1])) {
        line[len - 1] = '\0';
        len--;
    }
}

// 函数：处理 Makefile
void process_makefile(const char *filename, int verbose) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("错误: 无法打开 Makefile");
        exit(EXIT_FAILURE);
    }

    FILE *output_file = NULL;
    if (verbose) {
        output_file = fopen("Minimake_cleared.mk", "w");
        if (output_file == NULL) {
            perror("错误: 无法创建输出文件");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // 去除行尾换行符
        line[strcspn(line, "\n")] = '\0';

        // 清理行内容
        clean_line(line);

        // 过滤空行
        if (strlen(line) == 0) {
            continue;
        }

        // 输出清理后的内容
        if (verbose) {
            fprintf(output_file, "%s\n", line);
        } else {
            printf("%s\n", line);
        }
    }

    fclose(file);
    if (verbose) {
        fclose(output_file);
        printf("清理后的内容已保存到 Minimake_cleared.mk\n");
    }
}

int main(int argc, char *argv[]) {
    // 检查命令行参数
    int verbose = 0;
    if (argc == 2 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--verbose") == 0)) {
        verbose = 1;
    }

    // 检查 Makefile 是否存在
    FILE *file = fopen("Makefile", "r");
    if (file == NULL) {
        perror("错误: Makefile 不存在");
        return EXIT_FAILURE;
    }
    fclose(file);

    // 处理 Makefile
    process_makefile("Makefile", verbose);

    return 0;
}