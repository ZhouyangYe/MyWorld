cmake_minimum_required(VERSION 3.20.2)

# set the project name
project(MyWorld)

add_executable(${PROJECT_NAME}
  src/main.cpp
)

# add glfw
add_subdirectory(libs/glfw)

# add glm
add_subdirectory(libs/glm)

# add TOML
add_subdirectory(libs/tomlplusplus)

# add bgfx
add_subdirectory(libs/bgfx.cmake)

# add include directories
target_include_directories(${PROJECT_NAME}
  PUBLIC src
)

target_link_libraries(${PROJECT_NAME}
  glfw
  glm
  tomlplusplus_tomlplusplus
  bgfx
  bx
  bimg
)

# set c++ version
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
