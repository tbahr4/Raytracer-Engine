file(GLOB WORLD_SRC CONFIGURE_DEPENDS *.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${WORLD_SRC})
get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/${FOLDER_NAME})