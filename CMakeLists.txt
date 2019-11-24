cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_STANDARD 17)

project (hl3_d)

file(GLOB SRCS "*.cpp" "*.hpp" "*.h" "*.c")
add_executable(hl3 ${SRCS})
