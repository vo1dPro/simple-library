# 项目开发帮助文档

> ============================================================
>
> **注意：由于时间限制，此文档由 AI 直接生成（有删减），仅供参考，具体问题请咨询领队现干，主办方保有最终解释权。**
>
> ============================================================

## 简易图书馆管理系统

**公共基础项目说明文档**  
**版本：1.2**
**适用对象：2025 年开发队新干**
**项目周期：共 9 周**
**核心目标：通过实战掌握 C 语言核心能力，夯实数据结构与算法基础**

### 一、项目概述

#### 1.1 为什么选择本项目？

-   **教学价值**：
    -   90%计算机专业核心课程（操作系统、数据库）依赖本项目涉及的**指针操作、内存管理、文件 I/O**能力。
    -   通过**链表实现**强制暴露指针难点（避免用静态数组“偷懒”），快速提升调试能力。
-   **能力检验**：
    -   若无法完成本项目，将**禁止参与自命题项目**（基础不牢者需补训）。
-   **新干必读**：
    > 本项目不是“写一个小程序”，而是**系统性训练**。所有小组任务完全一致，结果将直接对比，**公平检验真实能力**。现干（老成员）会指导，但**代码必须自己写**（现干代写核心逻辑将导致全组不合格）。

#### 1.2 项目范围

| **允许**                                            | **禁止**                              |
| --------------------------------------------------- | ------------------------------------- |
| • C23 标准库（`stdio.h`, `stdlib.h`, `string.h`等） | • C++特性（如`class`、`std::vector`） |
| • GCC 12+ + CMake 3.25+ 工具链                      | • 非标准库扩展（如`<conio.h>`）       |
| • 命令行交互（无 GUI）                              | • 使用 Python/Shell 脚本替代核心逻辑  |
| • 二进制文件持久化                                  | • 仅内存模拟（必须写入磁盘）          |

### 二、开发环境配置（第 1 周必须完成）

#### 2.1 工具安装

```bash
# Linux/MacOS（推荐）
sudo apt install gcc cmake valgrind  # Ubuntu/Debian
brew install gcc cmake valgrind      # macOS

# Windows（通过WSL2）
# 在Microsoft Store安装“Ubuntu”，进入后执行：
sudo apt update && sudo apt install build-essential cmake valgrind
```

#### 2.2 验证环境

```bash
gcc --version    # 必须 ≥12.0
cmake --version  # 必须 ≥3.25
valgrind --version # 必须 ≥3.20
```

> **关键提示**：
>
> -   使用 **VS Code + C/C++ 扩展** 编写代码（[配置指南](https://code.visualstudio.com/docs/cpp/config-linux)）。
> -   所有代码必须通过 **CMake** 构建（禁用`gcc main.c`直接编译）。

### 三、核心功能实现要求（必须 100%完成）

#### 3.1 图书录入（`data.c`）

-   **需求**：
    ```c
    // 链表节点定义（不可修改）
    typedef struct Book {
        char isbn[20];
        char title[100];
        char author[50];
        int stock;     // 库存量
        int loaned;    // 借阅量
        struct Book* next;
    } BookNode;
    ```
    -   `int add_book(BookNode** head, const char* isbn, const char* title, ...)`
        -   **ISBN 冲突检测**：若 ISBN 已存在，返回`-1`并打印`"Error: ISBN [xxx] already exists"`。
        -   **动态内存**：必须用`malloc`创建新节点，否则扣 15 分。
-   **示例交互**：
    ```plaintext
    > add 9787532781234 "三体" 刘慈欣 5
    Book added successfully.
    > add 9787532781234 "流浪地球" 刘慈欣 3
    Error: ISBN 9787532781234 already exists
    ```

#### 3.2 图书查询（`logic.c`）

-   **需求**：
    -   `BookNode* search_by_isbn(BookNode* head, const char* isbn)`
        -   精确匹配 ISBN，返回节点指针（未找到返回`NULL`）。
    -   `BookNode* search_by_keyword(BookNode* head, const char* keyword)`
        -   **模糊搜索**：`keyword`匹配`title`或`author`（使用`strstr`），返回匹配链表。
        -   禁止全表扫描后过滤（需在遍历时实时匹配）。
-   **关键陷阱**：
    > 必须处理空链表（`head == NULL`），否则运行时崩溃将扣 10 分。

#### 3.3 借阅记录（`store.c`）

-   **需求**：
    -   `void log_loan(const char* isbn, int quantity)`
        -   将借阅记录写入**二进制文件** `loan.bin`，格式：
            ```c
            typedef struct LoanLog {
                char isbn[20];
                int quantity;
                time_t timestamp; // 用time()获取当前时间
            } LoanLog;
            ```
    -   `void load_loans(BookNode* head)`
        -   从`loan.bin`加载历史记录，更新`BookNode`中的`loaned`和`stock`字段。
-   **验证方法**：
    ```bash
    valgrind --leak-check=full ./libsys  # 必须无内存泄漏
    hexdump -C loan.bin                  # 检查二进制内容
    ```

#### 3.4 排序统计（`logic.c`）

-   **需求**：
    -   `void sort_by_stock(BookNode** head)`
        -   **实现快速排序**（禁用`qsort`），按`stock`升序排列。
        -   提供伪代码：
            ```c
            int partition(BookNode** arr, int low, int high) {
                // 新干需补全：选择基准、交换节点指针
            }
            void quick_sort(BookNode** arr, int low, int high) {
                // 新干需补全：递归调用partition
            }
            ```
    -   `void generate_report(BookNode* head)`
        -   输出统计：库存>5 的图书数量、最热门图书（`loaned`最高）。

#### 3.5 数据导出（`store.c`）

-   **需求**：
    -   `void export_to_csv(const char* filename, BookNode* head)`
        -   生成 CSV 格式：`isbn,title,author,stock,loaned`（每行一条记录）。
    -   `void export_to_json(const char* filename, BookNode* head)`
        -   生成 JSON 格式：`[{"isbn":"...","title":"..."}, ...]`
        -   **禁止用第三方库**（需手动拼接字符串）。
-   **示例输出**（`books.csv`）：
    ```csv
    9787532781234,三体,刘慈欣,5,3
    9787532782345,流浪地球,刘慈欣,2,1
    ```

#### 3.6 持久化 JSON 格式说明

持久化 JSON 文件格式如下（示例）：

```json
{
	"metadata": {
		"version": "1.0",
		"created": "1726704000"
	},
	"books": [
		{
			"isbn": "9787532781234",
			"title": "三体",
			"author": "刘慈欣",
			"stock": 5,
			"loaned": 3
		},
		{
			"isbn": "9787532782345",
			"title": "流浪地球",
			"author": "刘慈欣",
			"stock": 2,
			"loaned": 1
		}
	]
}
```

##### 3.6.1 格式特点

1. **元数据**：包含版本和创建时间，便于未来格式升级
2. **结构化**：书籍数据以数组形式组织
3. **可读性**：格式化输出，便于人工查看
4. **完整性**：包含所有业务所需字段（包括`loaned`）

#### 3.6.2 与已有导出功能的区别

| 功能         | `persist_books_json`    | `export_to_json` |
| ------------ | ----------------------- | ---------------- |
| **目的**     | 系统内部状态持久化      | 外部数据导出     |
| **文件格式** | 包含元数据的完整结构    | 简化数组格式     |
| **可读性**   | 为系统加载优化          | 为外部工具优化   |
| **使用场景** | 程序启动/退出时自动调用 | 用户主动导出     |
| **内容**     | 包含`loaned`等内部状态  | 仅基础信息       |

> **关键提示**：新干需特别注意，`persist_books_json`与`export_to_json`是两个不同功能，不能相互替代。

### 四、技术规范（违反直接扣分）

#### 4.1 内存管理（占 25%分数）

-   **必须做到**：
    -   每次`malloc`有对应`free`（链表销毁函数`void destroy_list(BookNode* head)`）。
    -   用 Valgrind 检测：
        ```bash
        valgrind --leak-check=full ./libsys
        # 正确输出：==0== ERROR SUMMARY: 0 errors from 0 contexts
        ```
-   **扣分规则**：  
    | 问题类型 | 扣分 |
    | ---------------------- | ----- |
    | 每发现 1 次内存泄漏 | -3 分 |
    | 未实现`destroy_list()` | -15 分 |
    | Valgrind 检测未通过 | -20 分 |

#### 4.2 模块分离（占 20%分数）

| 文件                | 职责                                             | 禁止行为                         |
| ------------------- | ------------------------------------------------ | -------------------------------- |
| `data.h`/`data.c`   | 数据容器：链表操作（`add_book`, `destroy_list`） | • 调用`fopen`<br>• 包含业务逻辑  |
| `logic.h`/`logic.c` | 业务逻辑：查询、排序、统计                       | • 操作文件<br>• 直接打印结果     |
| `store.h`/`store.c` | 文件接口：`log_loan`, `export_to_csv`            | • 修改链表结构<br>• 处理用户输入 |
| `main.c`            | 程序入口：命令行解析、调用模块                   | • 实现核心算法<br>• 内存分配     |

> **验证命令**：
>
> ```bash
> grep -r "fopen" src/  # 仅允许在store.c中出现
> ```

#### 4.3 代码注释（占 15%分数）

-   **要求**：
    -   每个函数头必须有注释（功能、参数、返回值）。
    -   关键逻辑行注释（如快速排序的递归条件）。
    -   注释行占比 ≥20%（用`cloc`验证）：
        ```bash
        cloc --include-lang=C --report-file=report.txt src/
        # 检查 "Comment" 列是否 ≥ 20% of "Code"
        ```
-   **反例**：

    ```c
    // BAD: 无意义注释
    free(node); // free the node

    // GOOD: 说明为什么释放
    free(node); // Release memory to prevent leak after deletion
    ```

### 五、开发流程与交付物

#### 5.3 交付物标准

| 交付物       | 要求                                                                                                        | 示例                                            |
| ------------ | ----------------------------------------------------------------------------------------------------------- | ----------------------------------------------- |
| **设计文档** | • 数据结构图（链表/文件格式）<br>• 模块分工表（新干姓名+负责函数）                                          | [模板下载](https://example.com/design_template) |
| **测试报告** | • 10+测试用例（含边界用例）<br>• Valgrind 检测结果截图<br>• 问题解决过程                                    | 用例：`"add 978... (库存负数)" → 应报错`        |
| **演示视频** | • ≤5 分钟，命令行操作全程录屏<br>• 展示所有功能+错误处理                                                    | [示例视频](https://example.com/demo_sample)     |
| **Git 仓库** | • 分支：`main`（最终版）+ `dev`（开发中）<br>• 提交频率：≥3 次/周<br>• 信息规范：`feat: add search_by_isbn` | 检查：`git log --oneline -n 5`                  |

### 六、评分标准（满分 100 分）

#### 6.1 功能实现（60 分）

| 项目     | 满分 | 评分细则                                         |
| -------- | ---- | ------------------------------------------------ |
| 图书录入 | 12   | • 链表存储(4) • ISBN 冲突检测(5) • 无内存泄漏(3) |
| 图书查询 | 12   | • ISBN 精确搜索(4) • 模糊搜索(5) • 空链表处理(3) |
| 借阅记录 | 10   | • 二进制文件写入(5) • 历史记录加载(5)            |
| 排序统计 | 13   | • 快速排序实现(8) • 统计报告(5)                  |
| 数据导出 | 13   | • CSV 导出(6) • JSON 导出(7)                     |

#### 6.2 技术规范（40 分）

| 项目     | 满分 | 评分细则                                     |
| -------- | ---- | -------------------------------------------- |
| 内存管理 | 25   | • Valgrind 无泄漏(20) • 有 destroy_list()(5) |
| 模块分离 | 20   | • 无跨模块调用(15) • 接口清晰(5)             |
| 代码注释 | 15   | • 注释 ≥20%(10) • 关键逻辑注释(5)            |

> **不合格红线**：
>
> -   现干代写核心逻辑（如链表操作代码占比>30%）→ **0 分**
> -   未使用 CMake 构建 → **扣 20 分**
> -   交付物缺 1 项 → **项目不合格**

### 七、常见问题（FAQ）

**Q1：能否用数组模拟链表？**
A：**禁止**。必须用指针实现链表（否则功能分全扣）。

**Q2：JSON 导出必须 100%符合标准吗？**
A：需能被标准解析器（如 Python `json.load()`）读取，允许无缩进。

**Q3：Valgrind 报“Still reachable”算泄漏吗？**
A：不算（属于系统保留内存），但“Definitely lost”必须修复。

**Q4：测试用例可以只测正常流程吗？**
A：**必须包含边界用例**（如空链表、库存负数、文件写入失败），否则测试报告扣 10 分。
