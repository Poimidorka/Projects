cmake_minimum_required(VERSION 3.19)
project(archiver)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2 -Wall -Wextra ")
#-fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null
add_executable(archiver main.cpp cell.cpp tie_archiver.cpp node.cpp stream_class.cpp Executor.cpp ordered_queue.cpp)
