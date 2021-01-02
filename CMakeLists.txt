cmake_minimum_required(VERSION 3.12)

project(ray-tracing-practice LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(make-ppm
    make_ppm.cpp
    sphere.cpp
    hittable_list.cpp
)


