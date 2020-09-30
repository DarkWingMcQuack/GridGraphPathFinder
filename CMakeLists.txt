cmake_minimum_required(VERSION 3.14)

#get options and compiling env
include(cmake/options.cmake)
include(cmake/linker.cmake)
include(cmake/ccache.cmake)

project(GridGraphPathFinder)
project(GridGraphPathFinder LANGUAGES CXX)

#set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
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

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# needed for multithreading
find_package(Threads REQUIRED)

include(cmake/flags.cmake)

#############################################
## library with all the logic which
## can later be easily linked against test
#############################################
add_library(GridGraphPathFinderSrc "")

target_sources(GridGraphPathFinderSrc
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include/graph/GridGraph.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graph/GridGraphIterator.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graph/GridCorner.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graph/GridCellIterator.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graph/GridCell.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graph/Node.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graph/CellRowWalker.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graph/CellColumnWalker.hpp

  ${CMAKE_CURRENT_LIST_DIR}/include/utils/Utils.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/utils/Timer.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/utils/ProgramOptions.hpp

  ${CMAKE_CURRENT_LIST_DIR}/include/separation/Separation.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/separation/NodeSelection.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/separation/NodeSelectionCalculator.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/separation/WellSeparationChecker.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/separation/WellSeparationCalculator.hpp

  ${CMAKE_CURRENT_LIST_DIR}/include/pathfinding/Path.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/pathfinding/ManhattanDijkstra.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/pathfinding/NormalDijkstra.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/pathfinding/DijkstraQueue.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/pathfinding/Distance.hpp

  PRIVATE
  src/graph/GridGraph.cpp
  src/graph/GridGraphIterator.cpp
  src/graph/GridCell.cpp
  src/graph/GridCorner.cpp
  src/graph/GridCellIterator.cpp
  src/graph/Node.cpp
  src/graph/CellRowWalker.cpp
  src/graph/CellColumnWalker.cpp

  src/separation/Separation.cpp
  src/separation/NodeSelection.cpp

  src/utils/ProgramOptions.cpp

  src/pathfinding/Path.cpp
  src/pathfinding/ManhattanDijkstra.cpp
  src/pathfinding/NormalDijkstra.cpp
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
add_executable(GridGraphPathFinder src/main.cpp)

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
