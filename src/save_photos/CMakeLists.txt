cmake_minimum_required(VERSION 3.2)

project(objective_track)

# 查找opencv包
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# 自定义的头文件路径
# include_directories(${PROJECT_BINARY_DIR}/../include)

set(EXECUTABLE_OUTPUT_PATH /home/cjj/Desktop/RM_work/objective_track/bin)

add_executable(a.x save_photo.cpp)
target_link_libraries(a.x ${OpenCV_LIBS})