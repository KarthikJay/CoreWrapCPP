#[[
	This CMake file follows the guidelines listed here:
	https://hsf-training.github.io/hsf-training-cmake-webpage/aio/index.html
#]]

# Need CMake 3.19 minimum for Apple silicon support.
cmake_minimum_required (VERSION 3.19)

set(Boost_USE_STATIC_LIBS OFF)
cmake_policy(SET CMP0167 NEW)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS} ${COREWRAP_INCLUDE_DIRECTORIES})

# Test Properties
set(CMAKE_CXX_STANDARD					20)
set(CMAKE_CXX_EXTENSIONS				ON)
set(CMAKE_CXX_STANDARD_REQUIRED			ON)
set(CMAKE_CXX_VISIBILITY_PRESET			hidden)
enable_language(OBJCXX)
link_libraries(${Boost_LIBRARIES} ${CORE_FOUNDATION_LIB} ${CMAKE_DL_LIBS} CoreWrap++)
#add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
add_compile_options(-fdebug-info-for-profiling)
add_link_options(-coverage)

# Helper function to read Boost Test Suites and generate a list of all available test names
function(ctest_boost_adapter TARGET_NAME)
	get_target_property(SOURCE_LIST ${TARGET_NAME} SOURCES)
	foreach(SOURCE_FILE_NAME IN LISTS SOURCE_LIST)
		set(LINE_COUNT 0)
		file(STRINGS "${SOURCE_FILE_NAME}" TEST_LIST)
		foreach(LINE IN LISTS TEST_LIST)
			unset(TEST_CASE)
			MATH(EXPR LINE_COUNT "${LINE_COUNT}+1")
			if(NOT ${LINE} STREQUAL "")
				string(REGEX MATCH "BOOST_AUTO_TEST_SUITE\\( *([A-Za-z_0-9]+) *\\)" TEST_SUITE ${LINE})
				if(TEST_SUITE)
					string(REGEX REPLACE ".*\\( *([A-Za-z_0-9]+) *\\).*" "\\1" TEST_SUITE ${TEST_SUITE})
					set(CURRENT_TEST_SUITE ${TEST_SUITE})
				endif()
				string(REGEX MATCH "BOOST_AUTO_TEST_SUITE_END\\( *([A-Za-z_0-9]+) *\\)" UNSET_TEST_SUITE ${LINE})
				if(UNSET_TEST_SUITE)
					unset(CURRENT_TEST_SUITE)
				endif()
				string(REGEX MATCH "BOOST_AUTO_TEST_CASE\\( *([A-Za-z_0-9]+) *\\)" TEST_CASE ${LINE})
				if(TEST_CASE)
					string(REGEX REPLACE ".*\\( *([A-Za-z_0-9]+) *\\).*" "\\1" TEST_CASE ${TEST_CASE})
					add_test(NAME "${CURRENT_TEST_SUITE}/${TEST_CASE}" COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}")
					# Add metadata to test case so test explorers point at test case code instead of cmake code.
					get_filename_component(ABS_SOURCE_PATH ${SOURCE_FILE_NAME} ABSOLUTE)
					set_tests_properties("${CURRENT_TEST_SUITE}/${TEST_CASE}" PROPERTIES
										ENVIRONMENT "TEST_FILE=${ABS_SOURCE_PATH};TEST_LINE=${LINE_COUNT};BOOST_TEST_RUN_FILTERS=${CURRENT_TEST_SUITE}/${TEST_CASE}")
				endif()
			endif()
		endforeach()
	endforeach()
endfunction()

if(NOT WIN32)
	string(ASCII 27 Esc)
	set(ColourReset "${Esc}[m")
	set(ColourBold  "${Esc}[1m")
	set(Red         "${Esc}[31m")
	set(Green       "${Esc}[32m")
	set(Yellow      "${Esc}[33m")
	set(Blue        "${Esc}[34m")
	set(Magenta     "${Esc}[35m")
	set(Cyan        "${Esc}[36m")
	set(White       "${Esc}[37m")
	set(BoldRed     "${Esc}[1;31m")
	set(BoldGreen   "${Esc}[1;32m")
	set(BoldYellow  "${Esc}[1;33m")
	set(BoldBlue    "${Esc}[1;34m")
	set(BoldMagenta "${Esc}[1;35m")
	set(BoldCyan    "${Esc}[1;36m")
	set(BoldWhite   "${Esc}[1;37m")
endif()

# Test Targets
add_executable(cfbool-tests)
target_sources(cfbool-tests
				PRIVATE
					CFBooleanTest.cpp)
ctest_boost_adapter(cfbool-tests)

add_executable(cfnumber-tests)
target_sources(cfnumber-tests
				PRIVATE
					CFNumberTest.cpp)
ctest_boost_adapter(cfnumber-tests)

add_executable(cfstring-tests)
target_sources(cfstring-tests
				PRIVATE
					CFStringTest.cpp)
ctest_boost_adapter(cfstring-tests)

get_directory_property(TEST_TARGETS DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} BUILDSYSTEM_TARGETS)
set(PROFILE_RAW_TARGETS ${TEST_TARGETS})
list(TRANSFORM PROFILE_RAW_TARGETS APPEND ".profraw")

# Commands to generate raw profiling data from each test executables
foreach(TEST ${TEST_TARGETS})
	add_custom_command(	OUTPUT ${TEST}.profraw
						COMMAND "LLVM_PROFILE_FILE=$<TARGET_FILE_BASE_NAME:${TEST}>.profraw"
								$<TARGET_FILE:${TEST}>
								|| (exit 0) # This is needed in case tests fail, so underlying make system doesn't exit.
						WORKING_DIRECTORY ${PARENT_BUILD_DIR}
						COMMENT "${BoldCyan}Generating coverage data for ${TEST}...${ColourReset}"
						DEPENDS ${TEST}
						VERBATIM)
	string(APPEND RAW_PROFILES "${TEST}.profraw ")
endforeach()

# Target to generate raw profiling data
add_custom_target(	generate-cf-profraw
					DEPENDS ${PROFILE_RAW_TARGETS})

# Target to generate sparse profile data of all tests
add_custom_target(	generate-cf-profdata
					ALL
					COMMAND bash -c "xcrun llvm-profdata merge --sparse ${RAW_PROFILES}-o CF.profdata"
					WORKING_DIRECTORY ${PARENT_BUILD_DIR}
					COMMENT "${BoldCyan}Collating raw profiling data...${ColourReset}"
					DEPENDS generate-cf-profraw
					VERBATIM)

# Target to generate coverage report
#add_custom_command(	OUTPUT CFCoverage.html)
#add_custom_target(	generate-cf-coverage-report)


# TODO: Custom Profiler (Requires only pure ANSI C functions)
#[[
set_source_files_properties(CFBooleanTest.cpp PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -finstrument-functions -fpic")
add_library(CustomProfiler)
target_sources(CustomProfiler
				PRIVATE
					ProfilerInstrumentation.cpp)
target_link_libraries(cfbool-tests PRIVATE CustomProfiler)
#]]