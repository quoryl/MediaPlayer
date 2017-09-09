cmake_minimum_required(VERSION 3.8)

add_subdirectory(lib/googletest)
set(gtest_SOURCE_DIR, lib/googletest/)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(SOURCE_TEST_FILES testSrc/runAllTests.cpp testSrc/SongTest.cpp testSrc/SongFixture.cpp)
add_executable(runAllTests ${SOURCE_TEST_FILES})
target_link_libraries(runAllTests gtest gtest_main core)

#You have to link wxWidgets to google tests. Otherwise you'll get a bunch of undefined references

find_package(wxWidgets COMPONENTS core base media REQUIRED)
include(${wxWidgets_USE_FILE})
target_link_libraries(runAllTests ${wxWidgets_LIBRARIES})
