set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(unit_tests
  grid_graph_test.cpp
  simple_dijkstra_test.cpp
  main.cpp
  )

target_link_libraries(unit_tests LINK_PUBLIC
  gtest
  GridGraphPathFinderSrc
  )

target_include_directories(
  unit_tests PUBLIC
  gtest
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
  )

add_test(
  NAME unit_tests
  COMMAND
  ${CMAKE_BINARY_DIR}/test/unit_tests
  # late if you want to pass data into tests
  # this is handy
  # ${CMAKE_CURRENT_SOURCE_DIR}/resources/
  )


add_dependencies(unit_tests gtest-project)
