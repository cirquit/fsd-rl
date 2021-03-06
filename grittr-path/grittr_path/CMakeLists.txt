cmake_minimum_required(VERSION 2.8.7)
project(grittr_path)
set(CMAKE_BUILD_TYPE Release)

##################################
##         trdrop_lib           ##
##################################

# set flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# headers lie in the folder "include"
include_directories(include)

# set every src/*.cpp file as source
file(GLOB SOURCES "src/*.cpp")

# generate shared library from sources
# add_library(trdrop SHARED ${SOURCES})

# generate static library from sources
add_library(grittr_path STATIC ${SOURCES})

# use "sudo make install" to install the library to location "/usr/lib"
install(TARGETS grittr_path DESTINATION /usr/lib)

##################################
##       trdrop_lib tests       ##
##################################

# create an interface for catch
add_library(Catch INTERFACE)

# tell cmake where the header of catch lies
target_include_directories(Catch INTERFACE ../ext/)

# tell cmake where the tests lie
file(GLOB TEST_SOURCES "tests/*.cpp")
add_executable(grittr_path_tests ${TEST_SOURCES})

# create the test
enable_testing()
target_link_libraries(grittr_path_tests Catch)
add_test(NAME Test COMMAND grittr_path_tests)