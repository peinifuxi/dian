#include <stdio.h>
#include <string.h>

// 帮助信息
void print_help() {
    printf("用法: ./program [选项]\n");
    printf("选项:\n");
    printf("  --help      显示帮助信息\n");
    printf("  --version   显示版本信息\n");
}

// 版本信息
void print_version() {
    printf("程序版本: 1.0.0\n");
}

int main(int argc, char *argv[]) {
    // 如果没有提供任何参数
    if (argc == 1) {
        printf("若需其他功能请使用 --help 查看帮助信息。\n");
        return 1;
    }

    // 遍历所有参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help();
            return 0;
        } else if (strcmp(argv[i], "--version") == 0) {
            print_version();
            return 0;
        } else if (strcmp(argv[i], "--relation") == 0) {
            print_version();
            return 0;
        } else if (strcmp(argv[i], "--order") == 0) {
            print_version();
            return 0;
        } else if (strcmp(argv[i], "--cleanmk") == 0) {
            print_version();
            return 0;           
        } else {
            // 如果参数不是 --help 或 --version，则认为是错误参数
            printf("错误: 未知参数 '%s'\n", argv[i]);
            return 1;
        }
    }

    return 0;
}