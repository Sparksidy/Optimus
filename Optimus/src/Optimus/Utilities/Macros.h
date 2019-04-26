#pragma once

#define BIT(x) (1 << x)

#define TO_STRING(x) #x

#define OP_VULKAN_ASSERT(func, ...)\
	if (func(__VA_ARGS__) != VK_SUCCESS)\
	{\
		OP_CORE_ERROR("{0} ", TO_STRING(func), "failed");\
		std::terminate();\
	}