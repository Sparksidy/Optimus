#pragma once

#ifdef OP_PLATFORM_WINDOWS
	#ifdef OP_BUILD_DLL
		#define OPTIMUS_API __declspec(dllexport)
	#else
		#define OPTIMUS_API __declspec(dllimport)
	#endif
#else
	#error Optimus can fly only on Windows!
#endif

#define BIT(x) (1 << x)