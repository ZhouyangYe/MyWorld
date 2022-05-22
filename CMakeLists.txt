cmake_minimum_required(VERSION 3.20.2)

# set the project name
project(MyWorld)

add_executable(${PROJECT_NAME}
  libs/FastNoiseLite/Cpp/FastNoiseLite.h

  src/main.cpp
  src/Core/App.h
  src/Core/App.cpp
  src/Core/Util.h
  src/Core/Util.cpp
  src/Core/EventsHandler.h
  src/Core/EventsHandler.cpp
  src/Core/Time.h
  src/Core/Time.cpp
  src/Core/Cursor.h
  src/Core/Cursor.cpp
  src/Core/World/Chunk.h
  src/Core/World/Chunk.cpp
  src/Events/Event.h
  src/Events/KeyCode.h
  src/Events/KeyEvent.h
  src/Events/MouseCode.h
  src/Events/MouseEvent.h
  src/Renderer/Renderer.h
  src/Renderer/Renderer.cpp
  src/Renderer/Camera.h
  src/Renderer/Camera.cpp
  src/Renderer/Model/Blocks/Block.h
  src/Renderer/Model/Blocks/Block.cpp
  src/Renderer/Model/Blocks/Grass.h
  src/Renderer/Model/Blocks/Grass.cpp
  src/Renderer/Model/Blocks/Dirt.h
  src/Renderer/Model/Blocks/Dirt.cpp
  src/Renderer/Model/Texture.h
  src/Renderer/Model/Texture.cpp
  src/Renderer/Tools.h
  src/Renderer/Tools.cpp
  src/Window/Window.h
  src/Window/Window.cpp
  src/Renderer/Model/Blocks/Air.h
  src/Renderer/Model/Blocks/Air.cpp
  src/Renderer/Model/Blocks/Water.h
  src/Renderer/Model/Blocks/Water.cpp
  src/Core/Info.h
  src/Core/Info.cpp
  src/Core/World/World.h
  src/Core/World/World.cpp
  src/Renderer/Model/Blocks/WireFrame.h
  src/Renderer/Model/Blocks/WireFrame.cpp
  src/Renderer/Model/Model.h
  src/Renderer/Model/Model.cpp
  src/Renderer/Model/Characters/Player.h
  src/Renderer/Model/Characters/Player.cpp
  src/Physics/HitBox.h
  src/Physics/HitBox.cpp
  src/Core/Objects/PlayerObj.h
  src/Core/Objects/PlayerObj.cpp
  src/Core/Objects/BaseObj.h
  src/Core/Objects/BaseObj.cpp
  src/Physics/Handler.h
  src/Physics/Handler.cpp
  src/Core/World/TerrainGeneration.h
  src/Core/World/TerrainGeneration.cpp
  src/Core/World/ChunkGeneration.h
  src/Core/World/ChunkGeneration.cpp
  src/Core/World/GameState.h
  src/Core/World/GameState.cpp
)

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
  PUBLIC libs/FastNoiseLite/Cpp
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
