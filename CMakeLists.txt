#[[
	This CMake file follows the guidelines listed here:
	https://hsf-training.github.io/hsf-training-cmake-webpage/aio/index.html
#]]

# Need CMake 3.19 minimum for Apple silicon support.
cmake_minimum_required (VERSION 3.19)

# Extra Cmake debug helpers
include(CMakePrintHelpers)

# Compile commands:
# 1. rm -rf build/ && cmake -B build/ -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON -DCMAKE_BUILD_TYPE=Debug
# 2. VERBOSE=1 cmake --build ./build --config Debug &| tee compile.log
# 3. pushd ./build/tests/ && LLVM_PROFILE_FILE="cfbool.profraw" ./cfbool-tests && popd
# 4. pushd ./build/tests/ && xcrun llvm-profdata merge -sparse ./cfbool.profraw -o cfbool.profdata && xcrun llvm-cov show ./cfbool-tests -instr-profile=./cfbool.profdata && xcrun llvm-cov report ./cfbool-tests -instr-profile=./cfbool.profdata && popd

# Test command:
# 1. ctest --test-dir ./build/tests/

# Global Project Definition
project(
	CoreWrap++
	VERSION 0.0.3
	DESCRIPTION "C++ wrappers for Apple's Objective-C Core* Libraries"
	HOMEPAGE_URL "https://github.com/KarthikJay/CoreWrapCPP"
	LANGUAGES C CXX)

# Project options
option(BUILD_SHARED_LIBS	"Build a shared library."	OFF)
option(BUILD_TESTS			"Build tests."				OFF)
option(BUILD_EXAMPLES		"Build examples."			OFF)
option(BUILD_INTERNAL_SDK	"Use internal APPLE SDK's"  ON )

# Global Project Properties
set(CMAKE_CXX_STANDARD					20)
set(CMAKE_CXX_EXTENSIONS				OFF)
set(CMAKE_CXX_STANDARD_REQUIRED			ON)
set(CMAKE_CXX_VISIBILITY_PRESET			hidden)
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE 				"Release" CACHE STRING
      									"Choose the type of build, options are: \"Debug\" \"Release\" \"RelWithDebInfo\" \"MinSizeRel\"."
      									FORCE)
endif(NOT CMAKE_BUILD_TYPE)
set(CMAKE_SUPPRESS_DEVELOPER_ERRORS		FALSE CACHE BOOL
										"Marks AUTHOR_WARNING messages as errors and aborts.")
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS	TRUE CACHE BOOL
										"Hides AUTHOR_WARNING messages, to enable prefer running with -Wdev on cmake command.")
# Only exports compile_commands.json if generator is Unix makefiles or Ninja.
set(CMAKE_EXPORT_COMPILE_COMMANDS		ON)
# include PUBLIC directories
set(COREWRAP_INCLUDE_DIRECTORIES		include/)

# Platform Project Properties
if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
	string(APPEND SDK_PATH_COMMAND "xcrun --sdk ")
	if(${CMAKE_SYSTEM_NAME} MATCHES "iOS")
		string(APPEND SDK_PATH_COMMAND "iphoneos")
		set(CMAKE_OSX_ARCHITECTURES				"arm64e"	CACHE STRING
			"Comma seperated list of platform targets to build.")
		set(CMAKE_OSX_DEPLOYMENT_TARGET			"" 			CACHE STRING
			"Must be empty for iOS builds." FORCE)
		set(CMAKE_XCODE_ATTRIBUTE_SDKROOT	"iphoneos")
	else()
		string(APPEND SDK_PATH_COMMAND "macosx")
		set(CMAKE_XCODE_ATTRIBUTE_SDKROOT	"macosx")
	endif()
	if(BUILD_INTERNAL_SDK)
		string(APPEND SDK_PATH_COMMAND ".internal")
		string(APPEND CMAKE_XCODE_ATTRIBUTE_SDKROOT	".internal")
	endif()
	string(APPEND SDK_PATH_COMMAND " --show-sdk-path")
	execute_process (
		COMMAND bash -c ${SDK_PATH_COMMAND}
		OUTPUT_VARIABLE APPLEOS_SDK_PATH
		ERROR_QUIET
		COMMAND_ECHO NONE
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	set(CMAKE_OSX_SYSROOT ${APPLEOS_SDK_PATH})
	set(CMAKE_FRAMEWORK_PATH
		${CMAKE_OSX_SYSROOT}/System/Library/Frameworks
		${CMAKE_OSX_SYSROOT}/System/Library/PrivateFrameworks
		${CMAKE_OSX_SYSROOT}/Developer/Library/Frameworks)
endif()

# Global Project Targets
add_library(CoreWrap++)
target_sources(CoreWrap++
				PUBLIC
					include/ExportVisibility.h
					include/CF/TypeInterface.hpp
					include/CF/Boolean.hpp
					include/CF/Number.hpp
				PRIVATE
					src/CF/TypeInterface.cpp
					src/CF/Boolean.cpp
					src/CF/Number.cpp)

# Set compile options
target_compile_options(CoreWrap++
						PRIVATE
							"-Wall" "-Wextra" "-Wpedantic" "-fno-rtti" "-Wextra-semi" "-Wunused"
							"-Warray-bounds-pointer-arithmetic" "-Wconversion" "-fconstant-cfstrings")
if(${CMAKE_BUILD_TYPE} MATCHES "Deb")
	target_compile_options(CoreWrap++ PRIVATE "")
elseif(${CMAKE_BUILD_TYPE} MATCHES "Rel")
	target_compile_options(CoreWrap++ PRIVATE "-Werror")
endif()
# If tests are enabled, we want support for code coverage as well.
if(BUILD_TESTS)
	if (NOT ${CMAKE_BUILD_TYPE} MATCHES "Deb")
		message(WARNING "When building tests, please make sure to build with either \"Debug\" or \"RelWithDebInfo\"")
	endif()
	target_compile_options(CoreWrap++ PRIVATE -fprofile-instr-generate -fcoverage-mapping)
	target_link_options(CoreWrap++ PUBLIC -coverage)
	# Address Sanitizer for memory leak detection
	#target_sources(CoreWrap++ PRIVATE src/CF/AddressSanitizerOptions.cpp)
	#target_compile_options(CoreWrap++ PRIVATE -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls)
	#target_link_options(CoreWrap++ PUBLIC -fsanitize=address)
# Figure out gcovr wrapper call target
# Need to run ./cfbool-test once to generate the .gcda files
# gcovr -r=./ ./build/CMakeFiles/CoreWrap++.dir/src/CF/ --exclude-function-lines --exclude-throw-branches
# gcov --gcda=../CMakeFiles/CoreWrap++.dir/src/CF/Boolean.cpp.gcda --gcno=../CMakeFiles/CoreWrap++.dir/src/CF/Boolean.cpp.gcno ../../src/CF/Boolean.cpp

# Clang/LLVM test coverage support as well: https://clang.llvm.org/docs/SourceBasedCodeCoverage.html
# Note for macs, llvm toolchain is dictated by xcrun paths: https://github.com/official-stockfish/Stockfish/issues/2053
# How to read the coverage report: https://llvm.org/doxygen/structllvm_1_1coverage_1_1CoverageSegment.html
endif()

# Library additional folders to include
target_include_directories(CoreWrap++ 
							PUBLIC
								${CMAKE_SOURCE_DIR}/include/)

# Compiler dependent defines
#if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
#	target_sources(CoreWrap++ PRIVATE UnixVisibility.h)
#elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
#	target_sources(CoreWrap++ PRIVATE MicrosoftVisibility.h)
#endif()

# This might be a more modern way of linking proprietary libraries?
# Or at least for linking libraries that don't have a CMake module definition
#target_link_libraries(CDUT PRIVATE "-framework CMCaptureCore")
#target_link_libraries(CDUT PRIVATE "-framework CMCapture")
#target_link_libraries(CDUT PRIVATE "-framework CoreMedia")

# Apple Frameworks
find_library(CORE_FOUNDATION_LIB CoreFoundation REQUIRED)
if(NOT CORE_FOUNDATION_LIB)
	message(FATAL_ERROR "Apple Core Foundation Framework not found via CMake!")
endif()

# Enable Tests
if(BUILD_TESTS)
	add_subdirectory(tests)
endif()

# Enable Examples
if(BUILD_EXAMPLES)
	add_subdirectory(examples/RetainLeaker)
	add_subdirectory(examples/CFNumberPlayground)
endif()