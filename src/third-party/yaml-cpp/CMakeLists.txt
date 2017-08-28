##
# CMake settings reworked for use with enSync, written by Igor Siemienowicz
#
# Yaml-cpp is a third-party dependency of enSync.

# Specify all Cmake stuff related to yaml-cpp in this file.

file(GLOB YAML_CPP_CONTRIB_SRC src/contrib/*.cpp)
file(GLOB YAML_CPP_SRC src/*.cpp)
add_library(yaml-cpp ${YAML_CPP_CONTRIB_SRC} ${YAML_CPP_SRC})
