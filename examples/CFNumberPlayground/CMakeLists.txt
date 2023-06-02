# Need CMake 3.19 minimum for Apple silicon support.
cmake_minimum_required (VERSION 3.19)

project(
	cfnumberplayground
	VERSION 1.0.0
	DESCRIPTION "Example program to show edge cases of CFNumber functions"
	HOMEPAGE_URL "TBA"
	LANGUAGES C CXX)

add_executable(cfnumberplayground)

target_sources(cfnumberplayground
				PRIVATE
				CFNumberPlayground.cpp)
target_link_libraries(cfnumberplayground ${CORE_FOUNDATION_LIB})
