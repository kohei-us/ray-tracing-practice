cmake_minimum_required(VERSION 3.12)

project(ray-tracing-practice LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
find_package(Threads REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(make-ppm
    aabb.cpp
    bvh.cpp
    camera.cpp
    make_ppm.cpp
    material.cpp
    sphere.cpp
    moving_sphere.cpp
    hittable_list.cpp
)

target_link_libraries(make-ppm Threads::Threads)
