cmake_minimum_required(VERSION 3.15)

project(SFML-Input)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
	set(SFML_STATIC_LIBRARIES TRUE)
endif()
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(SFML-Input main.cpp)

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
