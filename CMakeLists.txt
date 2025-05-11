cmake_minimum_required(VERSION 3.27)
project(RaytracerEngine)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# CPM
file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

set(LIBS)


## Source Files
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)


## Dependencies

# SDL
CPMAddPackage("gh:libsdl-org/SDL#4c70aa3")
list(APPEND LIBS SDL3::SDL3)

# SDL_image
set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE) # Requires Perl and Nasm for libaom (AVIF image format)
CPMAddPackage("gh:libsdl-org/SDL_image#bcc97c0")
list(APPEND LIBS SDL3_image::SDL3_image)

## Setup executable
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})