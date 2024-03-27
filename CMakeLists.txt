cmake_minimum_required(VERSION 3.26)

set(project "prj")
project(${project})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
set(SFML_STATIC_LIBRARIES TRUE)

set(${project}_SOURCES main.cpp)
set(${project}_SOURCE_LIST ${${project}_SOURCES})

add_executable(${project} ${${project}_SOURCE_LIST})

set(SFML_DIR "C:\\Users\\kiril\\Desktop\\C++\\SFML-2.6.1\\lib\\cmake\\SFML")
target_link_libraries(${project} sfml-graphics sfml-window sfml-system  )

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML 2.5 REQUIRED graphics network audio window system)