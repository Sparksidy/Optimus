#pragma once

#define BIT(x) (1 << x)

#define TO_STRING(x) #x

#define OP_VULKAN_ASSERT(func, ...)\
	if (func(__VA_ARGS__) != VK_SUCCESS)\
	{\
		OP_CORE_ERROR("{0} ", TO_STRING(func), "failed");\
		std::terminate();\
	}

#define GET_GRAPHICS_SYSTEM()  dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))

#ifndef SafeDelete
#define SafeDelete(x) { delete x; x = 0; }
#endif

#ifndef SafeDeleteArr
#define SafeDeleteArr(x) { delete [] x; x = 0; }
#endif