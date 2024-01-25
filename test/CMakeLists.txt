include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(adder_example examples/adder.cc)

target_link_libraries(adder_example ${PROJECT_NAME})

# target_link_libraries(sparse_set_test GTest::gtest_main)
# target_link_libraries(static_gui_example GTest::gtest_main)

include(GoogleTest)
