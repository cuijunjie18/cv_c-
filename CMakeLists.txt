cmake_minimum_required(VERSION 3.2)

project(card_identify)

# 查找opencv包
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# 自定义的头文件路径
# include_directories(${PROJECT_BINARY_DIR}/../include)
include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(main ${SRC})
target_link_libraries(main ${OpenCV_LIBS})