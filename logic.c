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
void quick_sort(BookNode** head) {
    /* --------------------
     *       [要求]
     * 1. 递归实现快速排序
     * 2. 链表头作为参数
     * -------------------- */
}

void sort_by_stock(BookNode** head) {
}

void sort_by_loan(BookNode** head) {
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
