cmake_minimum_required(VERSION 3.25)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "")
project(file_mover LANGUAGES C)

add_executable(file_mover src/event_handler.c src/helper_downloads.c src/inotify_initializer.c
 src/magic_initializer.c src/main.c)


target_include_directories(file_mover PRIVATE src src/include)


 target_link_libraries(file_mover PRIVATE magic)