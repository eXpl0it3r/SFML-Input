cmake_minimum_required(VERSION 3.15)

project(SFML-Input)

set(CMAKE_CXX_STANDARD 17)

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(SFML-Input main.cpp)

# Static Runtime
if(WIN32 AND MSVC AND ${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.15.0")
	set_property(TARGET SFML-Input PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

target_link_libraries(SFML-Input sfml-graphics)

install(TARGETS SFML-Input DESTINATION .)
install(FILES "Tuffy.ttf" DESTINATION .)