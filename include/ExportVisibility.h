#pragma once

// Example taken from: https://gcc.gnu.org/wiki/Visibility
#if defined _WIN32 || defined __CYGWIN__
	#define CWPP_SHARED_IMPORT __declspec(dllimport)
	#define CWPP_SHARED_EXPORT __declspec(dllexport)
	#define CWPP_SHARED_LOCAL
#else
	#if __GNUC__ >= 4
		#define CWPP_SHARED_IMPORT __attribute__ ((visibility ("default")))
		#define CWPP_SHARED_EXPORT __attribute__ ((visibility ("default")))
		#define CWPP_SHARED_LOCAL __attribute__ ((visibility ("hidden")))
	#else
		#define CWPP_SHARED_IMPORT
		#define CWPP_SHARED_EXPORT
		#define CWPP_SHARED_LOCAL
	#endif
#endif

// CWPP_API definitions
#ifdef BUILD_STATIC
	#define CWPP_API CWPP_SHARED_LOCAL
#else
	#define CWPP_API CWPP_SHARED_EXPORT
#endif

// \note Not needed with CMake setting visibility default to hidden
// #define CWPP_HIDDEN CWPP_SHARED_LOCAL