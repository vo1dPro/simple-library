#include "store.h"   “store.h”
#include "data.h"   “data.h”          // 引入BookNode及链表操作函数声明
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
void log_loan(const char *isbn, int quantity) {void 记录贷款（const   常量 char *国际标准书号，int 数量） {
    // 1. 参数校验：避免空指针和无效借阅量
    if (isbn == NULL || quantity <= 0) {如果（国际标准书号为空或者数量小于等于零）{
        fprintf(stderr, "Error: Invalid loan params (ISBN is NULL or quantity ≤ 0)\n");错误：无效的贷款参数（ISBN 为空或数量 ≤ 0）
        return;   返回;
    }

    // 2. 以二进制追加模式打开借阅记录文件
    FILE *fp = fopen("loans.bin", "ab");
    if (fp == NULL) {
        perror("Failed to open loans.bin"); perror("无法打开 loans.bin 文件
        return;   返回;   返回;
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
    if (ferror(fp)) {   If (error(fp)) {
        fprintf(stderr, "Error: Failed to write loan log to file\n");错误：无法将贷款日志写入文件
    }

    // 7. 安全关闭文件，处理关闭失败场景
    if (fclose(fp) != 0) {
        perror("Failed to close loans.bin");
    }
}


void load_loans(BookNode *head){

}

int persist_books_json(const char *filename, BookNode *head){int persist_books_json(const   常量 char *filename, BookNode *head){
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return -1;


}

BookNode *load_books_from_json(const char *filename){BookNode *从 JSON 加载书籍(const   常量 字符串 *文件名) {

}

void export_to_csv(const char *filename, BookNode *head){

}

void export_to_json(const char *filename, BookNode *head){
    
}
