#include "data.h"
#include "logic.h"
#include "store.h"
#include <stdio.h>
#include <string.h>

#define PERSISTENCE_FILE "library_data.json"

/**
 * @brief 打印帮助信息
 */
void print_help() {
    printf("Library Management System\n");
    printf("Commands:\n");
    printf("  add <isbn> <title> <author> <stock>   - 添加新图书\n");
    printf("  search <keyword>                      - 通过关键词搜索\n");
    printf("  isbn <isbn>                           - 通过ISBN搜索\n");
    printf("  loan <isbn> <quantity>                - 记录借阅\n");
    printf("  sort stock                            - 按库存量降序排序\n");
    printf("  sort loan                             - 按借阅数降序排序\n");
    printf("  report                                - 生成报告\n");
    printf("  export csv <filename>                 - 导出为CSV文件\n");
    printf("  export json <filename>                - 导出为JSON文件\n");
    printf("  exit                                  - 退出程序\n");
}

/**
 * @brief 主命令解析循环
 */
void command_loop(BookNode** head) {
    char command[100];

    while (1) {
        printf("> ");
        if (!fgets(command, sizeof(command), stdin)) {
            break;
        }

        // 移除换行符
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }

        // 解析命令
        char cmd[20];
        sscanf(command, "%s", cmd, 20);

        if (strcmp(cmd, "exit") == 0) {
            break;
        } else if (strcmp(cmd, "help") == 0) {
            print_help();
        } else if (strcmp(cmd, "add") == 0) {
            // TODO: 解析add命令参数
            // 用sscanf提取参数，调用add_book
        } else if (strcmp(cmd, "search") == 0) {
            // TODO: 解析搜索关键词，调用search_by_keyword
        } else if (strcmp(cmd, "isbn") == 0) {
            // TODO: 解析ISBN，调用search_by_isbn
        } else if (strcmp(cmd, "loan") == 0) {
            // TODO: 解析loan命令，调用log_loan
        } else if (strcmp(cmd, "sort") == 0) {
            // TODO: 解析排序类型
        } else if (strcmp(cmd, "report") == 0) {
            generate_report(*head);
        } else if (strncmp(cmd, "export", 6) == 0) {
            // TODO: 解析导出命令
        } else {
            printf("未知命令。输入 'help' 查看用法。\n");
        }
    }
}

int main() {
    // 设置控制台编码为UTF-8以解决中文乱码问题
    system("chcp 65001");
    
    BookNode* head = NULL;

    // 尝试从持久化文件加载数据
    BookNode* loaded = load_books_from_json(PERSISTENCE_FILE);
    if (loaded) {
        head = loaded;
        printf("已从 %s 加载图书数据。\n", PERSISTENCE_FILE);
    } else {
        printf("未找到存在的图书数据。将以空数据库开始。\n");//No existing library data found. Starting with empty library.
    }

    // 加载历史借阅记录
    load_loans(head);

    printf("图书管理系统（输入 'help' 查看命令）\n");
    command_loop(&head);

    // 退出前保存数据
    printf("正在将图书数据保存至 %s ...\n", PERSISTENCE_FILE);
    if (persist_books_json(PERSISTENCE_FILE, head) == 0) {
        printf("保存数据成功。\n");
    } else {
        printf("警告：保存数据失败。\n");
    }

    // 清理资源
    destroy_list(head);

    return 0;
}
