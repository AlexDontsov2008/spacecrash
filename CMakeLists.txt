cmake_minimum_required(VERSION 3.2)


project(SpaceCrash)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/;${CMAKE_MODULE_PATH}")

find_package(SFML 2 COMPONENTS audio graphics window system network)

include_directories(${SFML_INCLUDE_DIR})



# Specific compiler options
if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++")
endif()

set (SRC
	Source/Application.cpp
	Source/Button.cpp
	Source/Command.cpp
	Source/CommandQueue.cpp
	Source/Component.cpp
	Source/Container.cpp
	Source/DataTables.cpp
	Source/Entity.cpp
	Source/GameOverState.cpp
	Source/GameState.cpp
	Source/Label.cpp
	Source/Main.cpp
	Source/MenuState.cpp
	Source/PauseState.cpp
	Source/Planet.cpp
	Source/Player.cpp
	Source/SceneNode.cpp
	Source/Sattelite.cpp
	Source/SettingsState.cpp
	Source/Spaceship.cpp
	Source/SpriteNode.cpp
	Source/State.cpp
	Source/StateStack.cpp
	Source/TitleState.cpp
	Source/TextNode.cpp
	Source/Utility.cpp
	Source/World.cpp
	Source/Main.cpp)


include_directories("Includes/")
add_executable(SpaceCrash ${SRC})

target_link_libraries(SpaceCrash ${SFML_LIBRARIES})

install(DIRECTORY ${CHAPTER_DIR}/Media
		DESTINATION ${PROJECT_NAME}
		PATTERN "CMakeLists.txt" EXCLUDE)











