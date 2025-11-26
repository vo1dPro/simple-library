#include "store.h"
#include <stdio.h>   // fopen/fwrite等文件操作函数
#include <stdlib.h>  // 内存分配（后续函数会用到）
#include <string.h>  // 字符串操作（后续函数会用到）
#include <errno.h>   // perror依赖的头文件

void log_loan(const char *isbn, int quantity {
    // 以追加模式打开二进制文件
  FILE *fp = fopen("loans.bin", "ab");
    if (fp == NULL) {
        perror("Failed to open loans.bin"); //若不能打开文件 提示报错
        return;
    }

    // 写入ISBN（固定长度20字节）和借阅数量
    fwrite(isbn, sizeof(char), 20, fp);
    fwrite(&quantity, sizeof(int), 1, fp);

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