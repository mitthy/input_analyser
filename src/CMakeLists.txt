add_subdirectory(distributions)

add_definitions(-std=c++11)

add_executable(input_analyser mathutils.cpp mathutils.h montecarlo.cpp montecarlo.h dataholder.cpp dataholder.h main.cpp)

target_link_libraries(input_analyser distributions)

set(EXECUTABLE_OUTPUT_PATH "../")
