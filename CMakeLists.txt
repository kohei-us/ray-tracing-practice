cmake_minimum_required(VERSION 3.12)

project(ray-tracing-practice LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
find_package(Threads REQUIRED)

file(TO_CMAKE_PATH ${PROJECT_SOURCE_DIR}/images/earthmap.jpg EARTH_IMAGE_PATH)
message(STATUS "earth image path: ${EARTH_IMAGE_PATH}")
add_compile_definitions(EARTH_IMAGE_PATH="${EARTH_IMAGE_PATH}")

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/stb
)

add_executable(make-ppm
    aabb.cpp
    aarect.cpp
    box.cpp
    bvh.cpp
    camera.cpp
    hittable.cpp
    hittable_list.cpp
    make_ppm.cpp
    material.cpp
    moving_sphere.cpp
    perlin.cpp
    sphere.cpp
    texture.cpp
)

target_link_libraries(make-ppm Threads::Threads)
