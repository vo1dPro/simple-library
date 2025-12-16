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
            // 解析add命令参数
            char isbn[20], title[100], author[50];
            int stock;
            if (sscanf(command, "add %s %s %s %d", isbn, title, author, &stock) == 4) {
                int result = add_book(head, isbn, title, author, stock);
                if (result == 0) {
                    printf("图书添加成功。\n");
                } else if (result == -1) {
                    printf("ISBN已存在，无法添加。\n");
                } else {
                    printf("添加失败，未知错误。\n");
                }
            } else {
                printf("参数格式错误。用法: add <isbn> <title> <author> <stock>\n");
            }
        } else if (strcmp(cmd, "search") == 0) {
            // 解析搜索关键词，调用search_by_keyword
            char keyword[100];
            if (sscanf(command, "search %s", keyword) == 1) {
                BookNode* results = search_by_keyword(*head, keyword);
                if (results == NULL) {
                    printf("未找到匹配的图书。\n");
                } else {
                    printf("搜索结果：\n");
                    printf("ISBN\t\t书名\t\t\t作者\t\t库存\t借阅\n");
                    BookNode* current = results;
                    while (current != NULL) {
                        printf("%s\t%s\t\t%s\t\t%d\t%d\n", 
                               current->isbn, current->title, current->author, current->stock, current->loaned);
                        current = current->next;
                    }
                    destroy_list(results); // 释放结果链表内存
                }
            } else {
                printf("参数格式错误。用法: search <keyword>\n");
            }
        } else if (strcmp(cmd, "isbn") == 0) {
            // 解析ISBN，调用search_by_isbn
            char isbn[20];
            if (sscanf(command, "isbn %s", isbn) == 1) {
                BookNode* book = search_by_isbn(*head, isbn);
                if (book == NULL) {
                    printf("未找到ISBN为 %s 的图书。\n", isbn);
                } else {
                    printf("找到图书：\n");
                    printf("ISBN: %s\n", book->isbn);
                    printf("书名: %s\n", book->title);
                    printf("作者: %s\n", book->author);
                    printf("库存: %d\n", book->stock);
                    printf("借阅: %d\n", book->loaned);
                }
            } else {
                printf("参数格式错误。用法: isbn <isbn>\n");
            }
        } else if (strcmp(cmd, "loan") == 0) {
            // 解析loan命令，调用log_loan
            char isbn[20];
            int quantity;
            if (sscanf(command, "loan %s %d", isbn, &quantity) == 2) {
                BookNode* book = search_by_isbn(*head, isbn);
                if (book == NULL) {
                    printf("未找到ISBN为 %s 的图书。\n", isbn);
                } else if (book->stock < quantity) {
                    printf("库存不足，当前库存：%d\n", book->stock);
                } else {
                    log_loan(isbn, quantity);
                    book->stock -= quantity;
                    book->loaned += quantity;
                    printf("借阅记录成功。\n");
                    printf("剩余库存：%d\n", book->stock);
                }
            } else {
                printf("参数格式错误。用法: loan <isbn> <quantity>\n");
            }
        } else if (strcmp(cmd, "sort") == 0) {
            // 解析排序类型
            char sort_type[20];
            if (sscanf(command, "sort %s", sort_type) == 1) {
                if (strcmp(sort_type, "stock") == 0) {
                    sort_by_stock(head);
                    printf("按库存升序排序完成。\n");
                } else if (strcmp(sort_type, "loan") == 0) {
                    sort_by_loan(head);
                    printf("按借阅量降序排序完成。\n");
                } else {
                    printf("未知的排序类型。用法: sort <stock|loan>\n");
                }
            } else {
                printf("参数格式错误。用法: sort <stock|loan>\n");
            }
        } else if (strcmp(cmd, "report") == 0) {
            generate_report(*head);
        } else if (strncmp(cmd, "export", 6) == 0) {
            // 解析导出命令
            char export_type[20], filename[100];
            if (sscanf(command, "export %s %s", export_type, filename) == 2) {
                if (strcmp(export_type, "csv") == 0) {
                    export_to_csv(filename, *head);
                } else if (strcmp(export_type, "json") == 0) {
                    export_to_json(filename, *head);
                } else {
                    printf("未知的导出类型。用法: export <csv|json> <filename>\n");
                }
            } else {
                printf("参数格式错误。用法: export <csv|json> <filename>\n");
            }
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
