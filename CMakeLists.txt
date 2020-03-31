cmake_minimum_required(VERSION 3.10)
project(Lexier)

set(CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/obj)

ADD_SUBDIRECTORY(src)

## 开启测试
#enable_testing()
#
## 添加测试目录
#add_subdirectory(test)

