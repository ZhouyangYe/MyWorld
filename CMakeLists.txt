cmake_minimum_required(VERSION 3.20.2)

# set the project name
project(MyWorld)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/Core/App.h
  src/Core/App.cpp
  src/Core/Util.h
  src/Core/Util.cpp
  src/Events/Event.h
  src/Events/KeyCode.h
  src/Events/KeyEvent.h
  src/Events/MouseCode.h
  src/Events/MouseEvent.h
  src/Renderer/Renderer.h
  src/Renderer/Renderer.cpp
  src/Renderer/Camera.h
  src/Renderer/Camera.cpp
  src/Window/Window.h
  src/Window/Window.cpp
  src/Renderer/Model/Block.h
  src/Renderer/Model/Block.cpp
  src/Core/EventsHandler.h
  src/Core/EventsHandler.cpp
  src/Core/Time.h
  src/Core/Time.cpp
  src/Core/Cursor.h
  src/Core/Cursor.cpp
  src/Renderer/Model/Grass.h
  src/Renderer/Model/Grass.cpp
 "src/Renderer/Model/Texture.h")

# add glfw
add_subdirectory(libs/glfw)

# add glm
add_subdirectory(libs/glm)

# add TOML
add_subdirectory(libs/tomlplusplus)

# add bgfx
add_subdirectory(libs/bgfx.cmake)

# add bgfx
add_subdirectory(libs/entt)

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
  EnTT
)

# set c++ version
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
