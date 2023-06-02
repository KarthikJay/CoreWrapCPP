#[[
	This CMake file follows the guidelines listed here:
	https://hsf-training.github.io/hsf-training-cmake-webpage/aio/index.html
#]]

# Need CMake 3.19 minimum for Apple silicon support.
cmake_minimum_required (VERSION 3.19)

# Disable extraneous ctest targets from being generated
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED ON)
include(CTest)
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS} ${COREWRAP_INCLUDE_DIRECTORIES})

# Global Project Targets
set(CMAKE_CXX_STANDARD					20)
set(CMAKE_CXX_EXTENSIONS				ON)
set(CMAKE_CXX_STANDARD_REQUIRED			ON)
set(CMAKE_CXX_VISIBILITY_PRESET			hidden)
#set(LLVM_PROFILE_FILE					"ctest.profraw")
link_libraries(${Boost_LIBRARIES} ${CORE_FOUNDATION_LIB} ${CMAKE_DL_LIBS} CoreWrap++)
add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
add_link_options(-coverage)

add_executable(cfbool-tests)
target_sources(cfbool-tests
				PRIVATE
					CFBooleanTest.cpp)
add_executable(cfnumber-tests)
target_sources(cfnumber-tests
				PRIVATE
					CFNumberTest.cpp)

#[[
set_source_files_properties(CFBooleanTest.cpp PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -finstrument-functions -fpic")
add_library(CustomProfiler)
target_sources(CustomProfiler
				PRIVATE
					ProfilerInstrumentation.cpp)
target_link_libraries(cfbool-tests PRIVATE CustomProfiler)
#]]

add_test(NAME "CF::Boolean Tests" COMMAND cfbool-tests)
add_test(NAME "CF::Number Tests" COMMAND cfnumber-tests)