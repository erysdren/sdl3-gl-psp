cmake_minimum_required(VERSION 3.5)

project(sdl3-gl-psp)

add_executable(${PROJECT_NAME} main.c)

include(FindPkgConfig)
pkg_search_module(SDL3 REQUIRED sdl3)
if(NOT PSP)
	pkg_search_module(GL REQUIRED gl)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL3_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL3_LIBRARIES})
if(NOT PSP)
	target_include_directories(${PROJECT_NAME} PRIVATE ${GL_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} PRIVATE ${GL_LIBRARIES})
endif()

if(PSP)
	create_pbp_file(
		TARGET ${PROJECT_NAME}
		ICON_PATH NULL
		BACKGROUND_PATH NULL
		PREVIEW_PATH NULL
		TITLE ${PROJECT_NAME}
	)
endif()
