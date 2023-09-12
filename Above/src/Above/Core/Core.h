#pragma once

#include <memory>

#ifdef AB_PLATFORM_WINDOWS

	#if AB_DYNAMIC_LINK
		#ifdef AB_BUILD_DLL
			#define ABOVE_API __declspec(dllexport)
		#else 
			#define ABOVE_API __declspec(dllimport)
		#endif
	#else
		#define ABOVE_API 
	#endif
#else
	#error ABOVE ONLY SUPPORTS WINDOWS!
#endif

#ifdef AB_DEBUG
	#define AB_ENABLE_ASSERTS
#endif

#ifdef AB_ENABLE_ASSERTS
	#define AB_ASSERT(assertion, ...)			{if(!(assertion)) { AB_ERROR("ASSERTION FAILED: {0}", __VA_ARGS__); __debugbreak(); } }
	#define AB_CORE_ASSERT(assertion, ...)		{if(!(assertion)) { AB_CORE_ERROR("ASSERTION FAILED: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define AB_ASSERT(assertion, ...) 
	#define AB_CORE_ASSERT(assertion, ...) 
#endif


#define BIT(x) (1 << x)

#define AB_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Above
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}