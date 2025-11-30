#include "store.h"
#include <stdio.h>   // fopen/fwrite等文件操作函数
#include <stdlib.h>  // 内存分配（后续函数会用到）
#include <string.h>  // 字符串操作（后续函数会用到）
#include <errno.h>   // perror依赖的头文件

void log_loan(const char *isbn, int quantity) {
    // 1. 参数校验：避免空指针和无效借阅量（数量≤0无意义）
    if (isbn == NULL || quantity <= 0) {
        fprintf(stderr, "Error: Invalid loan params (ISBN is NULL or quantity ≤ 0)\n");
        return;
    }

    // 2. 保留原文件打开逻辑（追加模式"ab"正确）
    FILE *fp = fopen("loans.bin", "ab");
    if (fp == NULL) {
        perror("Failed to open loans.bin"); 
        return;
    }

    // 3. 用strncpy确保ISBN不超过20字节（匹配BookNode的isbn字段长度）
    char loan_isbn[20] = {0}; // 初始化避免垃圾值
    strncpy(loan_isbn, isbn, sizeof(loan_isbn) - 1); // 留1字节存'\0'
    loan_isbn[sizeof(loan_isbn) - 1] = '\0'; // 强制终止符，防止溢出

    // 4. 保留原写入逻辑（固定长度写入，保证二进制文件格式统一）
    fwrite(loan_isbn, sizeof(char), 20, fp);
    fwrite(&quantity, sizeof(int), 1, fp);

    // 5. 校验写入结果（增强健壮性）
    if (ferror(fp)) {
        fprintf(stderr, "Error: Failed to write loan log to file\n");
    }

    fclose(fp);
}

void load_loans(BookNode *head){

}

int persist_books_json(const char *filename, BookNode *head){

}

BookNode *load_books_from_json(const char *filename){

}

void export_to_csv(const char *filename, BookNode *head){

}

void export_to_json(const char *filename, BookNode *head){
    
}