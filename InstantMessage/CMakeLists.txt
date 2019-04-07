cmake_minimum_required(VERSION 3.0)

find_package(Boost)
IF (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIR})
	link_directories(${Boost_LIBRARY_DIR})
endif()

find_package (Boost REQUIRED COMPONENTS system thread filesystem)

aux_source_directory(. InstantMessage_src)
add_library(InstantMessage SHARED ${InstantMessage_src})

target_link_libraries(InstantMessage ${Boost})
