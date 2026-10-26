FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests test_math.cpp)
target_link_libraries(unit_tests PRIVATE option_pricer GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)
