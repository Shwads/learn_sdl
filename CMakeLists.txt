cmake_minimum_required( VERSION 3.8 )

project( learn_sdl LANGUAGES CXX )

find_package( SDL2 REQUIRED )
find_package( SDL2_IMAGE REQUIRED )

set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set( SOURCE_LEARN_SDL
    src/main.cpp 
    src/LTexture.cpp 
    src/LTexture.h
)

include_directories(src)

message ( STATUS "Compiler ID: " ${CMAKE_CXX_COMPILER_ID})
message ( STATUS "Release flags: " ${CMAKE_CXX_FLAGS_RELEASE})
message ( STATUS "Debug flags: " ${CMAKE_CXX_FLAGS_DEBUG})

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall)
    add_compile_options(-Weffc++)
    add_compile_options(-Wextra)
    add_compile_options(-Wconversion)
    add_compile_options(-Wsign-conversion)
    add_compile_options(-Werror)

    add_compile_options(-Wnon-virtual-dtor) # Class has virtual functions, but its destructor is not virtual
    add_compile_options(-Wreorder) # Data member will be initialized after [other] data member
    add_compile_options(-Wsometimes-uninitialized) # Variable improperly initialized
    add_compile_options(-Wunused-variable) # Variable is defined but unused
endif()

add_executable( LearnSDL ${SOURCE_LEARN_SDL})
target_include_directories( LearnSDL PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_DIR} src )
target_link_libraries( LearnSDL PRIVATE ${SDL2_LIBRARIES} SDL2_image::SDL2_image )
