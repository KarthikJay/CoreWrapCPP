# Need CMake 3.19 minimum for Apple silicon support.
cmake_minimum_required (VERSION 3.19)

project(
	RetainLeaker
	VERSION 0.0.1
	DESCRIPTION "Example program leaking CoreFoundation objects via unbalanced retains/releases"
	HOMEPAGE_URL "TBA"
	LANGUAGES C CXX)

add_executable(retainleaker)

target_sources(retainleaker
				PRIVATE
					RetainLeaker.cpp)
target_link_libraries(retainleaker ${CORE_FOUNDATION_LIB})

# Can run leak test via below command on macs:
# leaks -atExit --nostacks -- ./build/examples/RetainLeaker/retainleaker
