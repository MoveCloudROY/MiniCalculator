file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(sim ${SOURCES})

target_include_directories(sim  PUBLIC include)
target_link_libraries(sim PRIVATE vsrc)
