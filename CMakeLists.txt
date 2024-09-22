cmake_minimum_required(VERSION 3.10)
project(pngToMap)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(pngToMap src/main.cpp)
target_link_libraries(pngToMap ${OpenCV_LIBS})

install(TARGETS pngToMap
        DESTINATION bin)