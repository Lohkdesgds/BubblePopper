cmake_minimum_required(VERSION 3.28)

project(MainApp VERSION 1.0)

file(GLOB_RECURSE MAIN_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    main.cpp
)

add_executable(
    ${PROJECT_NAME} WIN32
    ${MAIN_SRCS}
)

FetchContent_GetProperties(allegro5)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/impl
    ${ALLEGRO_ADDON_INCLUDE_DIRS}
    ${DEPS_INCLUDES}
)

message(STATUS "Allegro include directories: ${allegro5_SOURCE_DIR}/include, ${allegro5_BINARY_DIR}/include")

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC 
#    ${CURL_LIBRARIES} 
	allegro
	allegro_main
	allegro_image
	allegro_primitives
	allegro_color
	allegro_font
	allegro_ttf
	allegro_audio
	allegro_acodec
	allegro_memfile
	allegro_physfs
)

# Embedded files
# b_embed(${PROJECT_NAME} "assets/EMprint-Regular.ttf")
# b_embed(${PROJECT_NAME} "assets/misu.png")