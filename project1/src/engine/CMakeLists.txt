cmake_minimum_required(VERSION 3.16)

project(engine)

add_library(engine SHARED
       engine.cpp
       manager.cpp
       
       Keyboard.cpp
       Mouse.cpp

       Math/Math.cpp
       Math/Matrix.cpp
       Math/Quaternion.cpp
       Math/Vector.cpp

       Renderer/Camera.cpp
       Renderer/Mesh.cpp
       Renderer/Renderer.cpp
       Renderer/Shader.cpp
       Renderer/Shaders.cpp
       Renderer/Texture.cpp
       Renderer/Light.cpp
       Renderer/Object.cpp
       Renderer/SkyBox.cpp

       thirdParty/glad4.6/src/glad.c
       thirdParty/stb/stb_vorbis.c

       Window/Window.cpp
)

target_include_directories(engine PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/glad4.6/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/SDL3-3.4.12/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/stb/
)

if(UNIX)
   target_link_libraries(engine
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/SDL3-3.4.12/lib/linux/libSDL3.a
   )

endif()

if(WIN32)
   target_link_libraries(engine
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/SDL3-3.4.12/lib/windows/libSDL3.a

        setupapi
        winmm
        imm32
        version
   )

   target_link_options(engine PRIVATE -static)
endif()

target_compile_options(engine PRIVATE
        -O3            
        -ffast-math  
)

add_custom_command(
    TARGET engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:engine>" 
        "${CMAKE_CURRENT_SOURCE_DIR}/../../$<TARGET_FILE_NAME:engine>"

    COMMAND ${CMAKE_COMMAND} -E echo
        "[CMake Debug] Successfully copied engine"
)
