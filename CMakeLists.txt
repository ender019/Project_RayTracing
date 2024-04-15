cmake_minimum_required(VERSION 3.26)

set(project "prj")
project(${project})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
set(SFML_STATIC_LIBRARIES TRUE)

set(SOURCE
    utils/Geometry/Geometry.cpp
    utils/Character/Character.cpp
    utils/Interface/Interface.cpp
    utils/Application/Application.cpp
    utils/Application/Settings.cpp
	main.cpp
)

set(HEADERS
    utils/Geometry/Geometry.hpp
    utils/Character/Character.hpp
    utils/Interface/Interface.hpp
    utils/Application/Application.hpp
    utils/Application/Settings.hpp
)

add_executable(${PROJECT_NAME} ${SOURCE} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC utils/Geometry)
target_include_directories(${PROJECT_NAME} PUBLIC utils/Character)
target_include_directories(${PROJECT_NAME} PUBLIC utils/Interface)
target_include_directories(${PROJECT_NAME} PUBLIC utils/Application)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/utils/Materials/times.ttf ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/utils/Materials/Doom_OST.mp3 ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

set(SFML_DIR "C:\\Users\\kiril\\Desktop\\C++\\SFML-2.6.1\\lib\\cmake\\SFML")
target_link_libraries(${project} sfml-system sfml-graphics sfml-audio)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML 2.5 COMPONENTS graphics network audio window system REQUIRED)