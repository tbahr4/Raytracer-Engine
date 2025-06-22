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

## Properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set SDL3 build options
set(SDL3_DISABLE_AUDIO ON CACHE BOOL "" FORCE)
set(SDL3_DISABLE_JOYSTICK ON CACHE BOOL "" FORCE)
set(SDL3_DISABLE_HAPTIC ON CACHE BOOL "" FORCE)
set(SDL3_DISABLE_GAMEPAD ON CACHE BOOL "" FORCE)
set(SDL3_DISABLE_SENSOR ON CACHE BOOL "" FORCE)
set(SDL3_DISABLE_POWER ON CACHE BOOL "" FORCE)
set(SDL3_DISABLE_LOCALE ON CACHE BOOL "" FORCE)
set(SDL3_DISABLE_CPUINFO ON CACHE BOOL "" FORCE)
set(SDL3_DISABLE_FILE ON CACHE BOOL "" FORCE)
set(SDL3_DISABLE_SHARED ON CACHE BOOL "" FORCE)
set(SDL3_DISABLE_TESTS ON CACHE BOOL "" FORCE)


## Gather all files
file(GLOB_RECURSE SOURCE_FILES_ALL CONFIGURE_DEPENDS src/**/*.cpp)
file(GLOB_RECURSE HEADER_FILES_ALL CONFIGURE_DEPENDS 
	include/**/*.h 
	include/**/*.hpp
)



## Additional dependencies

# SDL
CPMAddPackage("gh:libsdl-org/SDL#4c70aa3")
list(APPEND LIBS SDL3::SDL3)

# SDL_image
set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE) # Requires Perl and Nasm for libaom (AVIF image format)
CPMAddPackage("gh:libsdl-org/SDL_image#bcc97c0")
list(APPEND LIBS SDL3_image::SDL3_image)




## Setup executable
add_executable(${PROJECT_NAME} src/main.cpp ${SOURCE_FILES_ALL} ${HEADER_FILES_ALL})
add_subdirectory(src)
include_directories(${CMAKE_SOURCE_DIR}/include) # Fix Intellisense pathing
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})




## Setup VS logical formatting

# Folder structure - Top Level
source_group("src" REGULAR_EXPRESSION "src/.*")
source_group("include" REGULAR_EXPRESSION "include/.*")

# Folder structure - src
source_group("src\\Renderer" REGULAR_EXPRESSION "src/Renderer/.*")
source_group("src\\World" REGULAR_EXPRESSION "src/World/.*")
source_group("src\\Player" REGULAR_EXPRESSION "src/Player/.*")
source_group("src\\Material Mgr" REGULAR_EXPRESSION "src/Material Mgr/.*")
source_group("src\\Input Mgr" REGULAR_EXPRESSION "src/Input Mgr/.*")
source_group("src\\Utilities" REGULAR_EXPRESSION "src/Utilities/.*")

# Folder structure - include
source_group("include\\Renderer" REGULAR_EXPRESSION "include/Renderer/.*")
source_group("include\\World" REGULAR_EXPRESSION "include/World/.*")
source_group("include\\Player" REGULAR_EXPRESSION "include/Player/.*")
source_group("include\\Material Mgr" REGULAR_EXPRESSION "include/Material Mgr/.*")
source_group("include\\Input Mgr" REGULAR_EXPRESSION "include/Input Mgr/.*")
source_group("include\\Utilities" REGULAR_EXPRESSION "include/Utilities/.*")