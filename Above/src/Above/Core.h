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


#ifdef HZ_ENABLE_ASSERTS
	#define AB_ASSERT(assertion, ...)			{if(!(x)) { AB_ERROR("ASSERTION FAILED: {0}", __VA_ARGS__); __debugbreak(); } }
	#define AB_CORE_ASSERT(assertion, ...)		{if(!(x)) { AB_CORE_ERROR("ASSERTION FAILED: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define AB_ASSERT(assertion, ...) 
	#define AB_CORE_ASSERT(assertion, ...) 
#endif


#define BIT(x) (1 << x)