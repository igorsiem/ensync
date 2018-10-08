# Simple build script for the fmt library

file(GLOB SRC "src/*.cc")
add_library(fmt ${SRC})
