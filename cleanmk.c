#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
        return;  // 返回 void，无需 EXIT_FAILURE
    }

    // 打开输出文件
    FILE *output_file = fopen("cleanmk", "w");
    if (output_file == NULL) {
        perror("错误: 无法创建输出文件");
        fclose(file);  // 关闭已打开的文件
        return;
    }

    // 逐行处理文件
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

        // 输出清理后的内容到文件
        fprintf(output_file, "%s\n", line);
    }

    // 关闭文件
    fclose(file);
    fclose(output_file);
    printf("清理后的内容已保存到 cleanmk\n");
}

