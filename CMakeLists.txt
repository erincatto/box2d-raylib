cmake_minimum_required(VERSION 3.22)
include(FetchContent)

project(game
	VERSION 1.0.0
	DESCRIPTION "Shows how to integrate Box2D with raylib"
	HOMEPAGE_URL "https://box2d.org"
	LANGUAGES C
)



set_property(GLOBAL PROPERTY USE_FOLDERS ON)

FetchContent_Declare(
	box2d
	GIT_REPOSITORY https://github.com/erincatto/box2c.git
	GIT_TAG main
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(box2d)

FetchContent_Declare(
	raylib
	GIT_REPOSITORY https://github.com/raysan5/raylib.git
	GIT_TAG master
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(raylib)

add_executable(game main.c)

target_link_libraries(game PRIVATE box2d raylib)
