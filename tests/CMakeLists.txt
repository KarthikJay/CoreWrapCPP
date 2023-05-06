#[[
	This CMake file follows the guidelines listed here:
	https://hsf-training.github.io/hsf-training-cmake-webpage/aio/index.html
]]

# Need CMake 3.19 minimum for Apple silicon support.
cmake_minimum_required (VERSION 3.19)

# Disable extraneous ctest targets from being generated
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED ON)
include(CTest)
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS} ${COREWRAP_INCLUDE_DIRECTORIES})

# Global Project Targets
#set(CMAKE_CXX_FLAGS_DEBUG				"-Wall -Wextra -Wpedantic -g")
set(CMAKE_CXX_STANDARD					20)
set(CMAKE_CXX_EXTENSIONS				OFF)
set(CMAKE_CXX_STANDARD_REQUIRED			ON)
set(CMAKE_CXX_VISIBILITY_PRESET			hidden)
link_libraries(${Boost_LIBRARIES} ${CORE_FOUNDATION_LIB} CoreWrap++)
add_executable(cfbool-tests)
target_sources(cfbool-tests
				PRIVATE
					CFBooleanTest.cpp)
add_executable(cfnumber-tests)
target_sources(cfnumber-tests
				PRIVATE
					CFNumberTest.cpp)
#target_link_libraries(cfbool-tests ${Boost_LIBRARIES} ${CORE_FOUNDATION_LIB} CoreWrap++)
#target_link_options(cfbool-tests PRIVATE -coverage)
#target_link_options(cfbool-tests PRIVATE -fprofile-instr-generate)
#target_compile_options(cfbool-tests PRIVATE -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls)
#target_link_options(cfbool-tests PRIVATE -fsanitize=address)
add_test(NAME "CF::Boolean Tests" COMMAND cfbool-tests)
add_test(NAME "CF::Number Tests" COMMAND cfnumber-tests)