#include <stdlib.h>

#ifndef LIBRARY_DATA_H
#define LIBRARY_DATA_H

/**
 * @brief 图书节点结构体
 *
 * 注意：必须使用此结构定义，不可修改
 */
typedef struct Book {
    char isbn[20];     // ISBN编号
    char title[100];   // 书名
    char author[50];   // 作者
    int stock;         // 库存量
    int loaned;        // 借阅量
    struct Book *next; // 指向下一节点
} BookNode;

/**
 * @brief 添加新书到链表
 *
 * @param head 链表头指针的指针
 * @param isbn ISBN编号
 * @param title 书名
 * @param author 作者
 * @param stock 库存量
 * @return int 0=成功, -1=ISBN已存在
 */
int add_book(BookNode **head, const char *isbn, const char *title, const char *author, int stock);

/**
 * @brief 通过ISBN精确查找图书
 *
 * @param head 链表头指针
 * @param isbn ISBN编号
 * @return BookNode* 找到的节点指针，NULL=未找到
 */
BookNode* search_by_isbn(BookNode *head, const char *isbn);

/**
 * @brief 按关键词（书名/作者）模糊搜索
 *
 * @param head 链表头指针
 * @param keyword 搜索关键词
 * @return BookNode* 匹配结果链表的头指针
 */
BookNode* search_by_keyword(BookNode *head, const char *keyword);

/**
 * @brief 销毁整个链表，释放内存
 *
 * @param head 链表头指针
 */
void destroy_list(BookNode *head);

/**
 * @brief 通过ISBN删除图书
 *
 * @param head 链表头指针的指针
 * @param isbn ISBN编号
 * @return int 0=成功删除, -1=未找到
 */
int delete_by_isbn(BookNode **head, const char *isbn);

#endif // LIBRARY_DATA_H