#include "data.h"
#include <string.h>
#include <stdlib.h>

int add_book(BookNode **head, const char *isbn, const char *title, const char *author, int stock){
    // 检查ISBN是否已存在
    if (search_by_isbn(*head, isbn) != NULL) {
        return -1;  // ISBN已存在，返回-1表示失败
    }
    
    // 分配内存创建新节点
    BookNode *new_node = (BookNode *)malloc(sizeof(BookNode));
    if (new_node == NULL) {
        return -1;  // 内存分配失败
    }
    
    // 填充节点数据
    strncpy(new_node->isbn, isbn, sizeof(new_node->isbn) - 1);
    new_node->isbn[sizeof(new_node->isbn) - 1] = '\0';
    
    strncpy(new_node->title, title, sizeof(new_node->title) - 1);
    new_node->title[sizeof(new_node->title) - 1] = '\0';
    
    strncpy(new_node->author, author, sizeof(new_node->author) - 1);
    new_node->author[sizeof(new_node->author) - 1] = '\0';
    
    new_node->stock = stock;
    new_node->loaned = 0;  // 初始借阅量为0
    new_node->next = NULL;
    
    // 如果链表为空，新节点成为头部
    if (*head == NULL) {
        *head = new_node;
    } else {
        // 否则插入到链表末尾
        BookNode *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    
    return 0;  // 成功返回0
}

BookNode* search_by_isbn(BookNode *head, const char *isbn){
    BookNode *current = head;
    while (current != NULL) {
        if (strcmp(current->isbn, isbn) == 0) {
            return current;  // 找到匹配的ISBN，返回节点指针
        }
        current = current->next;
    }
    return NULL;  // 未找到，返回NULL
}

BookNode* search_by_keyword(BookNode *head, const char *keyword){
    // 创建结果链表
    BookNode *result_head = NULL;
    BookNode *result_tail = NULL;
    
    BookNode *current = head;
    while (current != NULL) {
        // 检查书名或作者是否包含关键词
        if (strstr(current->title, keyword) != NULL || 
            strstr(current->author, keyword) != NULL) {
            
            // 创建新节点复制匹配的书籍信息
            BookNode *result_node = (BookNode *)malloc(sizeof(BookNode));
            if (result_node == NULL) {
                break;  // 内存分配失败，停止搜索
            }
            
            strcpy(result_node->isbn, current->isbn);
            strcpy(result_node->title, current->title);
            strcpy(result_node->author, current->author);
            result_node->stock = current->stock;
            result_node->loaned = current->loaned;
            result_node->next = NULL;
            
            // 将结果节点添加到结果链表
            if (result_head == NULL) {
                result_head = result_node;
                result_tail = result_node;
            } else {
                result_tail->next = result_node;
                result_tail = result_node;
            }
        }
        current = current->next;
    }
    
    return result_head;  // 返回结果链表头指针（可能为NULL）
}

void destroy_list(BookNode *head){
    BookNode *current = head;
    while (current != NULL) {
        BookNode *temp = current;
        current = current->next;
        free(temp);  // 释放当前节点的内存
    }
}

int delete_by_isbn(BookNode **head, const char *isbn){
    // 处理链表为空的情况
    if (*head == NULL) {
        return -1;  // 链表为空，未找到
    }
    
    // 如果要删除的是头节点
    if (strcmp((*head)->isbn, isbn) == 0) {
        BookNode *temp = *head;
        *head = (*head)->next;
        free(temp);
        return 0;  // 成功删除
    }
    
    // 在链表中查找并删除
    BookNode *current = *head;
    while (current->next != NULL) {
        if (strcmp(current->next->isbn, isbn) == 0) {
            BookNode *temp = current->next;
            current->next = current->next->next;
            free(temp);
            return 0;  // 成功删除
        }
        current = current->next;
    }
    
    return -1;  // 未找到该ISBN
}