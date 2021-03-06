cmake_minimum_required(VERSION 3.23.2 FATAL_ERROR)

set(PROJECT_NAME Battle_City)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
	src/main.cpp
	src/Render/ShaderProgram.cpp
	src/Render/ShaderProgram.h
	src/Res/ResManager.cpp
	src/Res/ResManager.h
	src/Res/stb_image.h
	src/Render/Texture2D.h
	src/Render/Texture2D.cpp
	src/Render/Sprite.cpp
	src/Render/Sprite.h
	src/Render/AnimatedSprite.cpp
	src/Render/AnimatedSprite.h
	src/Game/Game.h
	src/Game/Game.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

include_directories(external/glm)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)