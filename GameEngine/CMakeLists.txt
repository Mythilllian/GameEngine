# CMakeList.txt : CMake project for GameEngine, include source and define
# project specific logic here.
#

project(GameEngine LANGUAGES CXX)

# Add source to this project's executable.
set(SOURCE_FILES GameEngine.cpp Game.h Objects.h Window.h)
add_executable (GameEngineExec ${SOURCE_FILES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_target_properties(GameEngineExec PROPERTIES CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
