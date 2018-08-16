add_executable(nod_tests
    "catch.hpp"
    "test_helpers.hpp"
    "main.cpp"
    "tests/connection_tests.cpp"
    "tests/disconnect_all_slots_tests.cpp"
    "tests/general_tests.cpp"
    "tests/slot_count_tests.cpp"
    "tests/connection_tests.cpp"
    "tests/usage_examples.cpp"    
)

target_include_directories(nod_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(nod_tests nod::nod)
