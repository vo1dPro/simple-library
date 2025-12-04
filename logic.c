#include <stdio.h>
#include "data.h"

// TODO: 实现图书添加（检查ISBN重复、动态分配内存）
// BookNode *add_book(BookNode *head, const char *title, const char *author, const char *isbn, int stock) {
//     /* --------------------
//      *       [要求]
//      * 1. 检查ISBN是否重复
//      * 2. 使用malloc创建节点
//      * 3. 返回新链表头
//      * -------------------- */
//     return NULL;
// }

// TODO: 手写快速排序
// void quick_sort(BookNode** head) {
//     /* --------------------
//      *       [要求]
//      * 1. 递归实现快速排序
//      * 2. 链表头作为参数
//      * -------------------- */
// }

/**
 * @brief 按库存量升序排序（使用快速排序）
 *
 * @param head 链表头指针的指针
 */
void sort_by_stock(BookNode** head) {
    // 判断如果是空链表或单节点链表，则无需排序
    if(*head == NULL || (*head)->next == NULL) {
        return; 
    }

    // 快速排序的实现
    BookNode* pivot = *head; // 选择第一个节点作为基准节点
    BookNode* left = NULL;   // 小于基准节点的子链表
    BookNode* right = NULL;  // 大于等于基准节点的子链表
    for(BookNode* current = (*head)->next; current != NULL; current = current->next) {
        BookNode* next = current->next; // 先保存下一个节点
        // 根据库存量将节点分配到左右子链表，
        // 如果库存量小于基准节点，则插入左子链表，否则插入右子链表
        if(current->stock < pivot->stock) {
            // 从原链表中摘除节点
            current->next = left;
            // 从左子链表头的位置插入节点
            left = current;
        } else {
            current->next = right;
            right = current;
        }
    }

    // 递归排序左右子链表
    sort_by_stock(&left);
    sort_by_stock(&right);

    // 合并排序后的子链表和基准节点
    // 连接左子链表和基准节点
    if(left != NULL) {
        *head = left;
        // 遍历左子链表，找到左子链表的最后一个节点，把它的next拼到基准节点处
        BookNode* current = left;
        while(current->next != NULL) {
            current = current->next;
        }
        current->next = pivot;
    } else {// 左子链表为空，说明没有比基准节点更小的节点，则将基准节点作为新链表头
        *head = pivot;
    }

    // 连接基准节点和右子链表
    pivot->next = right;

}

/**
 * @brief 按借阅量降序排序
 *
 * @param head 链表头指针的指针
 */
void sort_by_loan(BookNode** head) {
    // 判断如果是空链表或单节点链表，则无需排序
    if(*head == NULL || (*head)->next == NULL) {
        return; 
    }

    // 快速排序的实现
    BookNode* pivot = *head; // 选择第一个节点作为基准节点
    BookNode* left = NULL;   // 大于基准节点的子链表
    BookNode* right = NULL;  // 小于等于基准节点的子链表
    for(BookNode* current = (*head)->next; current != NULL; current = current->next) {
        BookNode* next = current->next; // 先保存下一个节点
        // 根据借阅量将节点分配到左右子链表，
        // 如果借阅量大于基准节点，则插入左子链表，否则插入右子链表
        if(current->loaned > pivot->loaned) {
            // 从原链表中摘除节点
            current->next = left;
            // 从左子链表头插入节点
            left = current;
        } else {
            current->next = right;
            right = current;
        }
    }

    // 递归排序左右子链表
    sort_by_loan(&left);
    sort_by_loan(&right);

    // 合并排序后的子链表和基准节点
    // 连接左子链表和基准节点
    if(left != NULL) {
        *head = left;
        // 遍历左子链表，找到左子链表的最后一个节点，把它的next拼到基准节点处
        BookNode* current = left;
        while(current->next != NULL) {
            current = current->next;
        }
        current->next = pivot;
    } else { // 左子链表为空，说明没有比基准节点更大的节点，则将基准节点作为新链表头
        *head = pivot;
    }

    // 连接基准节点和右子链表
    pivot->next = right;
}

/**
 * @brief 生成统计报告
 * 输出统计：库存>5 的图书数量、最热门图书（loaned最高）。
 *
 * @param head 链表头指针
 */
void generate_report(BookNode* head) {
    // 如果链表为空，输出友好性提示，直接返回方法
    if (head == NULL) {
        printf("暂无图书记录，无法生成统计报告。\n");
        return;
    }

    // 否则，遍历链表统计所需数据
    int stock_gt_5_count = 0; // 声明变量，存储库存大于5的图书数量
    BookNode* most_hot_book = head; // 声明变量，存储最热门图书的指针，初始指向链表头
    for(BookNode* current = head; current != NULL; current = current->next) {
        // 统计库存大于5的图书数量
        if (current->stock > 5) {
            stock_gt_5_count++;
        }

        // 判断借阅量最大的图书，作为最热门图书
        if(current->loaned > (most_hot_book ? most_hot_book->loaned : -1)) {
            // 更新最热门图书
            most_hot_book = current;
        }
    }

    printf("统计报告：\n");
    printf("库存大于5的图书数量：%d\n", stock_gt_5_count);
    if (most_hot_book) {
        printf("最热门图书：\n");
        printf("  ISBN: %s\n", most_hot_book->isbn);
        printf("  书名: %s\n", most_hot_book->title);
        printf("  作者: %s\n", most_hot_book->author);
        printf("  库存: %d\n", most_hot_book->stock);
        printf("  借阅量: %d\n", most_hot_book->loaned);
    } else {
        printf("暂无图书借阅记录，无法获取最热门图书。\n");
    }
}
