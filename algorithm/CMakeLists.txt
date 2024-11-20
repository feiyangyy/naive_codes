cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(MyProject VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CLI11/include)
# 添加可执行文件
add_executable(union union.cc)
add_executable(sort basic_sort.cc)
add_executable(heap heap.cc)
add_executable(isc practice/inversion.cc)
add_executable(seq search/seq_search.cc)
# add_compile_options("-g3")
set(CMAKE_CXX_FLAGS "-g3")

# 如果有其他源文件，可以通过以下方式添加
# add_executable(MyExecutable main.cpp other_file.cpp)

# 包含目录
# target_include_directories(MyExecutable PRIVATE include)

# 链接库（如果需要）
# target_link_libraries(MyExecutable PRIVATE my_library)