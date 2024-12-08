cmake_minimum_required(VERSION 3.10)
project(objective_track)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package( OpenCV REQUIRED )
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(my_interfaces REQUIRED)
find_package(visualization_msgs REQUIRED)

# 导入头文件查找路径
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(/usr/include/eigen3) # 注意最前面的"/"不要忘了
include_directories(${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/)

# 导入源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)

# 生成可执行文件
add_executable(objective_track ${SRC})

# 连接库文件
ament_target_dependencies(
  objective_track
  "rclcpp"
  "visualization_msgs"
  "my_interfaces"
)

target_link_libraries(
  objective_track
  ${OpenCV_LIBS}
)

# 安装
install(TARGETS
  objective_track
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
