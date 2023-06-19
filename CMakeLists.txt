cmake_minimum_required(VERSION 3.14)
project(nuklear_test C)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

# Create the executable
add_executable(nuklear_test
    main.c
)

include_directories(${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})
target_link_libraries(nuklear_test PRIVATE m ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
