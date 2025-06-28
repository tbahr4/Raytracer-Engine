add_subdirectory(Renderer)
add_subdirectory(World)
add_subdirectory(Player)
add_subdirectory("Material Mgr")
add_subdirectory("Input Mgr")
add_subdirectory(Utilities)

# Add top-level files
file(GLOB TL_SRC CONFIGURE_DEPENDS *.cpp)
target_sources(${PROJECT_NAME} PRIVATE ${TL_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
