cmake_minimum_required(VERSION 3.14)

#get options and compiling env
include(cmake/options.cmake)
include(cmake/linker.cmake)
include(cmake/ccache.cmake)
include(cmake/flags.cmake)

project(GridGraphPathFinder)
project(GridGraphPathFinder LANGUAGES CXX)

#set C++ standard to C++17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

############################
###get external projects
############################
include(cmake/fmt.cmake)
include(cmake/tbb.cmake)
include(cmake/span.cmake)
include(cmake/CLI11.cmake)
include(cmake/gtest.cmake)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# needed for multithreading
find_package(Threads REQUIRED)

#############################################
## library with all the logic which
## can later be easily linked against test
#############################################
add_library(GridGraphPathFinderSrc "")

target_sources(GridGraphPathFinderSrc
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include/GridGraph.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/GridCell.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/GridCellIterator.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/Node.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/ProgramOptions.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/Path.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/SimpleDijkstra.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/ManhattanDijkstra.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/MultiTargetDijkstra.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/MultiTargetManhattanDijkstra.hpp

  PRIVATE
  src/GridGraph.cpp
  src/GridCell.cpp
  src/GridCellIterator.cpp
  src/Node.cpp
  src/ProgramOptions.cpp
  src/Path.cpp
  src/SimpleDijkstra.cpp
  src/ManhattanDijkstra.cpp
  src/MultiTargetDijkstra.cpp
  src/MultiTargetManhattanDijkstra.cpp
  )

# add the dependencies of the target to enforce
# the right order of compiling
add_dependencies(GridGraphPathFinderSrc fmt-project)
add_dependencies(GridGraphPathFinderSrc tbb-project)
add_dependencies(GridGraphPathFinderSrc span-lite-project)
add_dependencies(GridGraphPathFinderSrc CLI11-project)

# make headers available
target_include_directories(GridGraphPathFinderSrc PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  fmt
  tbb
  ${CLI11_INCLUDE_DIR}
  ${SPAN_LITE_INCLUDE_DIR}
  )

#link against libarys
target_link_libraries(GridGraphPathFinderSrc LINK_PUBLIC
  fmt
  tbb)

target_link_libraries(GridGraphPathFinderSrc PRIVATE
  ${CMAKE_THREAD_LIBS_INIT})


###############################
## THE ACTUAL BINARY
###############################
add_executable(GridGraphPathFinder main.cpp)

# make headers available
target_include_directories(GridGraphPathFinder PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  fmt
  tbb
  ${CLI11_INCLUDE_DIR}
  ${SPAN_LITE_INCLUDE_DIR}
  )

#link against libarys
target_link_libraries(GridGraphPathFinder LINK_PUBLIC
  GridGraphPathFinderSrc
  fmt
  tbb
  ${CMAKE_THREAD_LIBS_INIT})

# add the dependencies of the target to enforce
# the right order of compiling
add_dependencies(GridGraphPathFinder GridGraphPathFinderSrc)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif (BUILD_TESTS)
