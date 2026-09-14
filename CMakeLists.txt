cmake_minimum_required(VERSION 3.20)
project(NavCore VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Werror -pedantic)

add_library(navcore src/road_graph.cpp src/csv_loader.cpp src/router.cpp)
target_include_directories(navcore PUBLIC include)

add_executable(navcore_cli src/main.cpp)
target_link_libraries(navcore_cli PRIVATE navcore)

add_executable(navcore_benchmark tools/benchmark.cpp)
target_link_libraries(navcore_benchmark PRIVATE navcore)

enable_testing()
add_executable(navcore_tests tests/test_navcore.cpp)
target_link_libraries(navcore_tests PRIVATE navcore)
add_test(NAME navcore_tests COMMAND navcore_tests)
add_test(NAME cli_smoke COMMAND navcore_cli --map ${CMAKE_SOURCE_DIR}/data/sample_map.csv --from A --to F --algo astar --cost time)
set_tests_properties(navcore_tests cli_smoke PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
