cmake_minimum_required(VERSION 3.15)
project(EngineStudents CXX)

add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}" PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_library(engine-students
        Mesh.h Mesh.cpp
        Material.h Material.cpp
        )