cmake_minimum_required(VERSION 2.6)
project(input_analyser)

add_executable(input_analyser montecarlo.cpp dataholder.cpp main.cpp)

install(TARGETS input_analyser RUNTIME DESTINATION bin)
