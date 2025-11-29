#include "data.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
    /**
     * 根据ISBN删除单本图书
     * 算法：先检查头节点，然后遍历链表查找目标节点，找到后删除并释放内存
     * 时间复杂度：O(n)
     */
    
    // 链表为空，无法删除
    if (*head == NULL) {
        return -1;
    }
    
    // 特殊情况：删除的是头节点
    if (strcmp((*head)->isbn, isbn) == 0) {
        BookNode *temp = *head;
        *head = (*head)->next;  // 更新头指针
        free(temp);  // 释放旧头节点的内存
        return 0;
    }
    
    // 一般情况：在链表中查找并删除
    BookNode *current = *head;
    while (current->next != NULL) {
        if (strcmp(current->next->isbn, isbn) == 0) {
            BookNode *temp = current->next;
            current->next = current->next->next;  // 跳过要删除的节点
            free(temp);  // 释放目标节点的内存
            return 0;
        }
        current = current->next;
    }
    
    return -1;  // 遍历完整个链表仍未找到
}

int update_title(BookNode *head, const char *isbn, const char *new_title){
    /**
     * 根据ISBN查找图书，修改其书名
     * 步骤：
     *   1. 调用search_by_isbn查找目标图书
     *   2. 如果找到，使用strncpy安全地复制新书名（防止缓冲区溢出）
     *   3. 确保字符串以NULL结尾
     * 时间复杂度：O(n)，其中n为链表长度
     */
    
    BookNode *book = search_by_isbn(head, isbn);
    
    if (book == NULL) {
        return -1;  // ISBN不存在
    }
    
    // 使用strncpy防止缓冲区溢出，保留字符串结尾空间
    strncpy(book->title, new_title, sizeof(book->title) - 1);
    book->title[sizeof(book->title) - 1] = '\0';  // 强制添加字符串结尾符
    return 0;  // 修改成功
}

int update_author(BookNode *head, const char *isbn, const char *new_author){
    /**
     * 根据ISBN查找图书，修改其作者
     * 步骤：
     *   1. 调用search_by_isbn查找目标图书
     *   2. 如果找到，使用strncpy安全地复制新作者名（防止缓冲区溢出）
     *   3. 确保字符串以NULL结尾
     * 时间复杂度：O(n)，其中n为链表长度
     */
    
    BookNode *book = search_by_isbn(head, isbn);
    
    if (book == NULL) {
        return -1;  // ISBN不存在
    }
    
    // 使用strncpy防止缓冲区溢出，保留字符串结尾空间
    strncpy(book->author, new_author, sizeof(book->author) - 1);
    book->author[sizeof(book->author) - 1] = '\0';  // 强制添加字符串结尾符
    return 0;  // 修改成功
}

int update_stock(BookNode *head, const char *isbn, int new_stock){
    /**
     * 根据ISBN查找图书，修改其库存量
     * 步骤：
     *   1. 调用search_by_isbn查找目标图书
     *   2. 如果找到，直接更新库存数值（整数操作，无需特殊处理）
     * 时间复杂度：O(n)，其中n为链表长度
     */
    
    BookNode *book = search_by_isbn(head, isbn);
    
    if (book == NULL) {
        return -1;  // ISBN不存在
    }
    
    // 直接更新库存量
    book->stock = new_stock;
    return 0;  // 修改成功
}

int update_loaned(BookNode *head, const char *isbn, int new_loaned){
    /**
     * 根据ISBN查找图书，修改其借阅量
     * 步骤：
     *   1. 调用search_by_isbn查找目标图书
     *   2. 如果找到，直接更新借阅数值（整数操作，无需特殊处理）
     * 时间复杂度：O(n)，其中n为链表长度
     */
    
    BookNode *book = search_by_isbn(head, isbn);
    
    if (book == NULL) {
        return -1;  // ISBN不存在
    }
    
    // 直接更新借阅量
    book->loaned = new_loaned;
    return 0;  // 修改成功
}