cmake_minimum_required(VERSION 3.0)

project(Network)

include_directories(.)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lboost_system -lboost_filesystem")

find_package(Boost)
IF (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIR})
	link_directories(${Boost_LIBRARY_DIR})
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(FileTransmit)
add_subdirectory(InstantMessage)
add_subdirectory(configuration)
add_subdirectory(DataPool)

aux_source_directory(./network/ network_src)
add_executable(network ${network_src})
find_package (Boost REQUIRED COMPONENTS system thread filesystem)
set (ProjectLibrary configuration FileTransmit InstantMessage)
target_link_libraries(network ${Boost} ${ProjectLibrary})

