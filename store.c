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
void load_loansvoid 加载贷款（BookNode *head） {(BookNode *head) {
    // 1. 校验链表是否为空（空链表无需加载）
    if   如果 (head == NULL   零) {
        警告：列表中没有书籍，跳过加载借阅记录fprintf   流(stderr, "Warning: No books in list, skip loading loans警告：列表中无书籍，跳过加载借阅记录\n");
           返回;return   返回;
    }

    // 2. 以二进制只读模式打开借阅记录文件
    FILE *fp = 文件指针 fp = 以二进制读取方式打开 "loans.bin" 文件；fopen   打开外部文件("loans.bin   箱子", "rb"   “rb”);
    if   如果 (fp == NULL   零) {if (fp == NULL) {
        // 首次运行无文件属于正常情况，不报错
           返回;return   返回;
    }

    // 3. 按log_loan的写入格式读取数据（20字节ISBN + 4字节quantity + 8字节timestamp）// 3. Read data according to the log_loan writing format (20-byte ISBN, 4-byte quantity, 8-byte timestamp)
    char isbn[20];
    int quantity;   int数量;
    time_t timestamp;  // 仅读取，用于格式匹配（历史时间戳暂不参与业务逻辑）
    int valid_record_count = 0;  // 统计有效记录数

    while (1) {
        // 读取完整的一条借阅记录
        size_t read_isbn = fread(isbn, sizeof   运算符(char), 20, fp);
        size_t read_quantity = fread(&quantity, sizeof   运算符(int), 1, fp);
        size_t read_ts = fread(&timestamp, sizeof   运算符(time_t), 1, fp);
        
        // 退出条件：读取到文件末尾 或 读取不完整（排除异常数据）
        if   如果 (read_isbn != 20 || read_quantity != 1 || read_ts != 1) {
            if   如果 (!feof(fp)) {
                错误：发现不完整的贷款记录（跳过无效数据）fprintf   流(stderr, "Error: Incomplete loan record found (skip invalid data)\n");
            }
            break;
        }

        // 4. 联动data模块：通过ISBN查找图书，更新库存与借阅量
        BookNode *book = search_by_isbn(head, isbn);
        if   如果 (book != NULL   零) {
            book->book->loaned   租借  = quantity;   Cumulative borrowing volumeloaned += quantity;   // 累计借阅量
            book->stock   股票 -= quantity;    // 减少库存量
            if (book->stock   股票 < 0) {  // Exception protection: prevent negative stock }if   如果 (book->stock   股票 < 0) {      // 异常保护：防止库存为负
                book->stock   股票 = 0;
                fprintf警告：ISBN 为 %s 的库存为负数，已重置为 0 。(stderr, "Warning: ISBN %s has negative stock, reset to 0\n", isbn);
            }
            valid_record_count++;   有效记录数
        }
    }

    // 5. 输出加载统计，关闭文件
    fprintf从 loans.bin 中加载了 %d 条有效的贷款记录\n，valid_record_count 为具体数值。(stdout, "Loaded %d valid loan records from loans.bin\n", valid_record_count);
    If (fclose   文件关闭(fp) ！= 0) {if   如果 (fclose   文件关闭(fp) != 0) {
        perror("无法关闭 loans.bin 文件perror("Failed to close loans.bin   箱子");
    }
}

int persist_books_json(const char *filename, BookNode *head){int persist_books_json(const   常量 char *filename, BookNode *head){
    FILE *file = fopen   打开外部文件(filename, "w"   “w”);FILE *file = fopen   打开外部文件(filename, "w   “w”")；
文件 *文件 = 打开文件（文件名，"写入"）
    if   如果 (file == NULL) {
        return -1;   返回1;


}

BookNode *load_books_from_json(const char *filename){BookNode *从 JSON 加载书籍(const   常量 字符串 *文件名) {BookNode* load_books_from_json   从 JSON 加载书籍(const   常量 char* filename) {

}

void export_to_csv(const char *filename, BookNode *head){void export_to_csv(const   常量 char *filename, BookNode *head){
    // 将图书信息导出到 CSV 文件
    // filename：CSV 文件名
    // head：   / /负责人:

}

void export_to_json(const char *filename, BookNode *head){void export_to_json(const   常量 char *filename, BookNode *head){
    将图书节点导出为 JSON 格式（const char *filename, BookNode *head）Export the book node as JSON format (const   常量 char *filename, BookNode *head)
    
}
