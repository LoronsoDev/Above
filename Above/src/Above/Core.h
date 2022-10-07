#pragma once

#ifdef AB_PLATFORM_WINDOWS

	#ifdef AB_BUILD_DLL
		#define ABOVE_API __declspec(dllexport)
	#else 
		#define ABOVE_API __declspec(dllimport)
	#endif
#else
	#error ABOVE ONLY SUPPORTS WINDOWS!
#endif