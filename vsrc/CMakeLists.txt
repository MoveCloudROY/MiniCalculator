cmake_policy(SET CMP0079 NEW)

verilate(
    sim COVERAGE TRACE
    INCLUDE_DIRS "${CMAKE_BINARY_DIR}/../vsrc"
    VERILATOR_ARGS -x-assign fast
    SOURCES top.v
)