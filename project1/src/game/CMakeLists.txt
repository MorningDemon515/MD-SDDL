cmake_minimum_required(VERSION 3.16)

project(game)

add_executable(game
       Main.cpp
       Color.cpp
)

target_include_directories(game PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../engine/
)

target_link_libraries(game
        engine
)

if(WIN32)
   target_link_options(game PRIVATE -static)
endif()

add_custom_command(
    TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:game>" 
        "${CMAKE_CURRENT_SOURCE_DIR}/../../$<TARGET_FILE_NAME:game>"

    COMMAND ${CMAKE_COMMAND} -E echo
        "[CMake Debug] Successfully copied game"
)
