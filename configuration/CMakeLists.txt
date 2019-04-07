cmake_minimum_required(VERSION 3.0)

aux_source_directory(. configuration_src)
add_library(configuration SHARED ${configuration_src})

