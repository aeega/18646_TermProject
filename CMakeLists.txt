cmake_minimum_required(VERSION 3.10.0)

# I named the project "wilkes", because 18646 is the zip code for 
# Wilkes-Barre, Pennsylvania

# Declares a project name. This will create a number of useful variables
# such as wilkes_SRC_DIR. It also declares what languages are going
# to be used.
project(wilkes LANGUAGES C CXX)

# CMake projects are often included as modules in other cmake projects,
# and when that happens, you don't want to run any kind of global
# initialization or configuration stuff, since that would be the
# responsibility of the parent project. This if/else block makes sure
# we only configure conan if we're the top-level project, not being
# included by something else. 
# Here we're just configuring conan - other things you might do here
# is enable testing, and including the GoogleTest framework.
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(STATUS "about to do conan cmake run")
    include("cmake/conan.cmake")
    conan_cmake_run(CONANFILE conanfile.txt
                    BASIC_SETUP
                    BUILD missing
                    OPTIONS ${WILKES_CONAN_OPTIONS})
    include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
endif()

# This finds the OpenCV package - it will be an error if it is not
# found. We've configured conan such that conan dependencies will always
# be found and preferred over anything globally installed on your system
find_package(OpenCV REQUIRED)

# This (optionally) finds OpenMP. Just comment this out if you don't 
# need it. If it is found, the appropriate flags will be added for 
# compilation and linking by the utils functions
find_package(OpenMP)

# This is a file that contains some useful macros for building
# libraries, executables. It mostly just gathers compilation flags 
# and config options into a single location
include("cmake/wilkes_utils.cmake")

# Descend into a project subdirectory. Projects are usually organized
# such that the project code lives in a subdirectory of its own name,
# and this will in turn allow the code to have qualified include 
# paths - "wilkes/some_header.h" rather than the more error-prone
# "some_header.h"
add_subdirectory(wilkes)
