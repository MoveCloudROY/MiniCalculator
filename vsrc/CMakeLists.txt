add_library(vsrc STATIC)

verilate(
    vsrc COVERAGE TRACE
    INCLUDE_DIRS "${CMAKE_BINARY_DIR}/../vsrc"
    VERILATOR_ARGS -x-assign fast
    SOURCES top.v
)