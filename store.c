#include "store.h"
#include "data.h"          // 引入BookNode及链表操作函数声明
#include <stdio.h>         // 文件操作函数
#include <stdlib.h>        // 内存分配与释放
#include <string.h>        // 字符串操作
#include <errno.h>         // 错误码处理
#include <time.h>          // 时间戳获取


/**
 * @brief 记录借阅操作到二进制文件（loans.bin）
 * @param isbn ISBN编号（非空，长度≤19）
 * @param quantity 借阅数量（正整数）
 */
void log_loan(const char *isbn, int quantity) {
    // 1. 参数校验：避免空指针和无效借阅量
    if (isbn == NULL || quantity <= 0) {
        fprintf(stderr, "Error: Invalid loan params (ISBN is NULL or quantity ≤ 0)\n");
        return;
    }

    // 2. 以二进制追加模式打开借阅记录文件
    FILE *fp = fopen("loans.bin", "ab");
    if (fp == NULL) {
        perror("Failed to open loans.bin"); 
        return;
    }

    // 3. 安全拷贝ISBN（匹配BookNode的isbn字段长度，防止溢出）
    char loan_isbn[20] = {0};  // 初始化避免垃圾值
    strncpy(loan_isbn, isbn, sizeof(loan_isbn) - 1);  // 留1字节存'\0'
    loan_isbn[sizeof(loan_isbn) - 1] = '\0';  // 强制终止符，确保字符串安全

    // 4. 获取当前时间戳（匹配help.md中LoanLog结构体的时间字段要求）
    time_t timestamp = time(NULL);

    // 5. 按固定格式写入二进制数据（ISBN+数量+时间戳，保证读写格式统一）
    fwrite(loan_isbn, sizeof(char), 20, fp);    // 写入20字节ISBN
    fwrite(&quantity, sizeof(int), 1, fp);      // 写入4字节借阅量
    fwrite(&timestamp, sizeof(time_t), 1, fp);  // 写入8字节时间戳

    // 6. 校验写入结果，增强健壮性
    if (ferror(fp)) {
        fprintf(stderr, "Error: Failed to write loan log to file\n");
    }

    // 7. 安全关闭文件，处理关闭失败场景
    if (fclose(fp) != 0) {
        perror("Failed to close loans.bin");
    }
}



/**
 * @brief 从二进制文件加载历史借阅记录，更新图书库存与借阅量
 * @param head 图书链表头指针（非空）
 */

/**
 * @brief 从二进制文件加载历史借阅记录，更新图书库存与借阅量
 * @param head 图书链表头指针（非空）
 */
void load_loans(BookNode *head) {
    // 1. 校验链表是否为空（空链表无需加载）
    if (head == NULL) {
        fprintf(stderr, "Warning: No books in list, skip loading loans\n");
        return;
    }

    // 2. 以二进制只读模式打开借阅记录文件
    FILE *fp = fopen("loans.bin", "rb");
    if (fp == NULL) {
        // 首次运行无文件属于正常情况，不报错
        return;
    }

    // 3. 按log_loan的写入格式读取数据（20字节ISBN + 4字节quantity + 8字节timestamp）
    char isbn[20];
    int quantity;
    time_t timestamp;  // 仅读取，用于格式匹配（历史时间戳暂不参与业务逻辑）
    int valid_record_count = 0;  // 统计有效记录数

    while (1) {
        // 读取完整的一条借阅记录
        size_t read_isbn = fread(isbn, sizeof(char), 20, fp);
        size_t read_quantity = fread(&quantity, sizeof(int), 1, fp);
        size_t read_ts = fread(&timestamp, sizeof(time_t), 1, fp);
        
        // 退出条件：读取到文件末尾 或 读取不完整（排除异常数据）
        if (read_isbn != 20 || read_quantity != 1 || read_ts != 1) {
            if (!feof(fp)) {
                fprintf(stderr, "Error: Incomplete loan record found (skip invalid data)\n");
            }
            break;
        }

        // 4. 联动data模块：通过ISBN查找图书，更新库存与借阅量
        BookNode *book = search_by_isbn(head, isbn);
        if (book != NULL) {
            book->loaned += quantity;   // 累计借阅量
            book->stock -= quantity;    // 减少库存量
            if (book->stock < 0) {      // 异常保护：防止库存为负
                book->stock = 0;
                fprintf(stderr, "Warning: ISBN %s has negative stock, reset to 0\n", isbn);
            }
            valid_record_count++;
        }
    }

    // 5. 输出加载统计，关闭文件
    fprintf(stdout, "Loaded %d valid loan records from loans.bin\n", valid_record_count);
    if (fclose(fp) != 0) {
        perror("Failed to close loans.bin");
    }
}


/**
 * @brief 持久化书籍信息到JSON文件（系统内部使用，含元数据）
 * @param filename 输出文件名（非空）
 * @param head 图书链表头指针
 * @return int 0=成功, -1=失败
 */
int persist_books_json(const char *filename, BookNode *head) {
    // 1. 参数校验：文件名不能为空
    if (filename == NULL) {
        fprintf(stderr, "Error: Invalid JSON filename (NULL pointer)\n");
        return -1;
    }

    // 2. 以覆盖写模式打开JSON文件
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Failed to open JSON file for persistence");
        return -1;
    }

    // 3. 写入JSON头部（含元数据，匹配help.md中持久化格式要求）
    fprintf(fp, "{\n");
    fprintf(fp, "  \"metadata\": {\n");
    fprintf(fp, "    \"version\": \"1.0\",\n");
    fprintf(fp, "    \"created\": \"%ld\"\n", (long)time(NULL));  // 时间戳（秒级）
    fprintf(fp, "  },\n");
    fprintf(fp, "  \"books\": [\n");

    // 4. 遍历链表，写入每本图书的完整信息
    BookNode *current = head;
    int is_first = 1;  // 控制JSON数组逗号分隔，避免末尾多余逗号
    while (current != NULL) {
        if (!is_first) {
            fprintf(fp, ",\n");
        }
        is_first = 0;

        // 写入单本图书的JSON对象（字段与BookNode完全对应）
        fprintf(fp, "    {\n");
        fprintf(fp, "      \"isbn\": \"%s\",\n", current->isbn);
        fprintf(fp, "      \"title\": \"%s\",\n", current->title);
        fprintf(fp, "      \"author\": \"%s\",\n", current->author);
        fprintf(fp, "      \"stock\": %d,\n", current->stock);
        fprintf(fp, "      \"loaned\": %d\n", current->loaned);
        fprintf(fp, "    }");

        current = current->next;
    }

    // 5. 写入JSON尾部，完成结构闭合
    fprintf(fp, "\n  ]\n");
    fprintf(fp, "}\n");

    // 6. 刷新缓冲区并关闭文件，处理关闭失败场景
    fflush(fp);  // 确保数据完全写入磁盘
    int close_result = fclose(fp);
    if (close_result != 0) {
        perror("Failed to close JSON file");
        return -1;
    }

    return 0;  // 成功返回0
}


/**
 * @brief 导出图书数据到CSV文件（外部使用，简化格式）
 * @param filename 输出文件名
 * @param head 图书链表头指针
 */
void export_to_csv(const char *filename, BookNode *head) {
    // 1. 参数校验：文件名和链表头指针非空
    if (filename == NULL || head == NULL) {
        fprintf(stderr, "Error: Invalid params for CSV export (filename NULL or empty list)\n");
        return;
    }

    // 2. 以覆盖写模式打开CSV文件
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Failed to open CSV file for export");
        return;
    }

    // 3. 写入CSV表头（外部工具可直接解析）
    fprintf(fp, "isbn,title,author,stock,loaned\n");

    // 4. 遍历链表，写入图书数据（字符串用双引号包裹，避免逗号干扰）
    BookNode *current = head;
    while (current != NULL) {
        fprintf(fp, "\"%s\",\"%s\",\"%s\",%d,%d\n",
                current->isbn,
                current->title,
                current->author,
                current->stock,
                current->loaned);
        current = current->next;
    }

    // 5. 刷新缓冲区并关闭文件，处理关闭失败场景
    fflush(fp);
    if (fclose(fp) != 0) {
        perror("Failed to close CSV file");
    } else {
        fprintf(stdout, "Successfully exported %d books to %s\n", get_book_count(head), filename);
    }
}


void export_to_json(const char *filename, BookNode *head){void export_to_json(const   常量 char *filename, BookNode *head){
    将图书节点导出为 JSON 格式（const char *filename, BookNode *head）Export the book node as JSON format (const   常量 char *filename, BookNode *head)
    
}
