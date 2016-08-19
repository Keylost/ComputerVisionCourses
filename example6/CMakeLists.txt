cmake_minimum_required(VERSION 2.8)

IF (WIN32)
#path to opencv build directory
set(OpenCV_DIR C:/opencv/opencv/build)
set(OpenCV_INCLUDES ${OpenCV_DIR}/include)
endif(WIN32)

set(PROJECT example)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
# 	add_definitions(-g)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

find_package(OpenCV)

set(SOURCES main.cpp
)

include_directories(
	${OpenCV_INCLUDES}
)

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH TRUE)

add_executable(${PROJECT} ${SOURCES})

target_link_libraries(${PROJECT}
	${OpenCV_LIBS} 
)

IF (MSVC)
set(CMAKE_CXX_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
endif(MSVC)
