cmake_minimum_required(VERSION 3.30)
project(ARCS-Utilities
        VERSION 0.0.1
        DESCRIPTION "Utilities library for ARCS my robotics research at UC Riverside")

set(CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_FLAGS "-W -Wall -Wextra")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(SPDLOG_PATH /usr/local/include/spdlog)

add_library(TestModule SHARED
        include/Modules/IModule.h
        src/Modules/IModule.cpp
        modules/ModuleManagerTest/TestModule.h
        modules/ModuleManagerTest/TestModule.cpp)


add_executable(TestLoader
        ${SPDLOG_PATH}/spdlog.h
        include/Modules/IModule.h
        src/Modules/IModule.cpp
        include/SimpleServer/LibraryLoader/SOLoader.h
        util/libraryTest.cpp)

target_include_directories(TestLoader PRIVATE
        ${SPDLOG_PATH}
        include/SimpleServer/LibraryLoader
        util/TestLibrary)

target_link_libraries(TestLoader ${CMAKE_DL_LIBS})


add_executable(TestServer
        ${SPDLOG_PATH}/spdlog.h
        include/Modules/IModule.h
        src/Modules/IModule.cpp
        include/SimpleServer/LibraryLoader/SOLoader.h
        include/SimpleServer/Core/ServerCore.h
        src/SimpleServer/Core/ServerCore.cpp
        src/SimpleServer/Core/ServerModuleManager.cpp
        util/ServerTest/ServerTest.cpp)

target_include_directories(TestServer PRIVATE
        ${SPDLOG_PATH})

target_link_libraries(TestServer PRIVATE
        yaml-cpp
        ${CMAKE_DL_LIBS})