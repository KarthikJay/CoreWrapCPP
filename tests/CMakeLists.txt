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
add_executable(cfbool-tests)
target_sources(cfbool-tests
				PRIVATE
					CFBooleanTest.cpp)
target_compile_options(cfbool-tests PRIVATE -Wall -Wextra -Wpedantic -g)
target_link_libraries(cfbool-tests ${Boost_LIBRARIES} ${CORE_FOUNDATION_LIB} CoreWrap++)
#target_link_options(cfbool-tests PRIVATE -coverage)
#target_link_options(cfbool-tests PRIVATE -fprofile-instr-generate)
#target_compile_options(cfbool-tests PRIVATE -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls)
#target_link_options(cfbool-tests PRIVATE -fsanitize=address)
add_test(NAME "CF::Boolean Tests" COMMAND cfbool-tests)
