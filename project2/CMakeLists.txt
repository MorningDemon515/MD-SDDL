cmake_minimum_required(VERSION 4.3)
project(testVulkan)

set(CMAKE_CXX_STANDARD 11)

add_executable(testVulkan main.cpp)

if(UNIX)
      target_link_libraries(testVulkan PRIVATE vulkan)
endif()

if(WIN32)
     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

     target_link_options(testVulkan PRIVATE -static-libgcc -static-libstdc++)
     target_link_libraries(testVulkan PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libvulkan-1.dll.a)
endif()