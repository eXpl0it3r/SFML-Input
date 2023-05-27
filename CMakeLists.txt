cmake_minimum_required(VERSION 3.15)

option(BUILD_SFML "Fetch and build SFML from source" NO)

project(SFML-Input)

set(CMAKE_CXX_STANDARD 17)

# Build or find SFML
if (BUILD_SFML)
    include(FetchContent)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
    FetchContent_MakeAvailable(SFML)
else()
    if(WIN32)
       set(SFML_STATIC_LIBRARIES TRUE)
    endif()
    find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
endif()

add_executable(SFML-Input
    src/Application.cpp
    src/Application.hpp
    src/KeyboardView.cpp
    src/KeyboardView.hpp
    src/main.cpp
    src/ranges.hpp
    src/ShinyText.cpp
    src/ShinyText.hpp
    src/strings.cpp
    src/strings.hpp
)

# Static Runtime
if(WIN32)
    if(MSVC)
        set_property(TARGET SFML-Input PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    endif()
endif()

target_link_libraries(SFML-Input sfml-graphics sfml-audio)

install(TARGETS SFML-Input DESTINATION .)
install(DIRECTORY resources DESTINATION .)
