cmake_minimum_required(VERSION 3.2)

project(objective_track)

# 查找opencv包
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(EXECUTABLE_OUTPUT_PATH /home/cjj/Desktop/RM_work/objective_track/bin)

add_executable(m.x mouse_click.cpp)
target_link_libraries(m.x ${OpenCV_LIBS})