cmake_minimum_required(VERSION 3.16)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

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
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/utils/Application/shader.frag ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

target_link_libraries(CMakeSFMLProject PRIVATE sfml-graphics sfml-audio)
target_compile_features(CMakeSFMLProject PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET CMakeSFMLProject
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:CMakeSFMLProject>
        VERBATIM)
endif()

install(TARGETS CMakeSFMLProject)


