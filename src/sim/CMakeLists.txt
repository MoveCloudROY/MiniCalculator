add_executable(sim src/sim_main.cpp)

target_link_libraries(sim PRIVATE vsrc)
target_include_directories(sim PRIVATE ${VERILATE_INCLUDE_DIRS})
