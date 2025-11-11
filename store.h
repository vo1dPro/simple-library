#ifndef LIBRARY_STORE_H
#define LIBRARY_STORE_H

#include "data.h"

/**
 * @brief 记录借阅操作到二进制文件
 *
 * @param isbn ISBN编号
 * @param quantity 借阅数量
 */
void log_loan(const char *isbn, int quantity);

/**
 * @brief 从二进制文件加载历史记录
 *
 * @param head 链表头指针
 */
void load_loans(BookNode *head);

/**
 * @brief 持久化书籍信息到JSON文件（系统内部使用）
 *
 * @param filename 输出文件名
 * @param head 链表头指针
 * @return int 0=成功, -1=失败
 */
int persist_books_json(const char *filename, BookNode *head);

/**
 * @brief 从JSON文件恢复书籍信息
 *
 * @param filename 输入文件名
 * @return BookNode* 恢复后的链表头指针（NULL表示失败）
 */
BookNode *load_books_from_json(const char *filename);

/**
 * @brief 导出图书数据到CSV文件（外部使用）
 *
 * @param filename 输出文件名
 * @param head 链表头指针
 */
void export_to_csv(const char *filename, BookNode *head);

/**
 * @brief 导出图书数据到JSON文件（外部使用）
 *
 * @param filename 输出文件名
 * @param head 链表头指针
 */
void export_to_json(const char *filename, BookNode *head);

#endif // LIBRARY_STORE_H