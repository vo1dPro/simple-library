cmake_minimum_required(VERSION 3.25.0)
project(book_management VERSION 0.1.0 LANGUAGES C)

# 使用C23标准
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 把所有 .c 文件都加进可执行程序里
add_executable(book_management
    main.c
    data.c
    logic.c
    store.c
)

include(CTest)
enable_testing()
