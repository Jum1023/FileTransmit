cmake_minimum_required(VERSION 3.0)

aux_source_directory(. DataPool_src)
add_library(DataPool SHARED ${DataPool_src})
