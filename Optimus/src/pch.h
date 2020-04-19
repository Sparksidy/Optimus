#pragma once
//STL
#include <iostream>
#include <utility>
#include <functional>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <fstream>
#include <optional>
#include <filesystem>
#include <thread>

//GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <gtc/matrix_transform.hpp>
#include <glm.hpp>


//Engine
#include <Optimus/Utilities/Macros.h>
#ifdef OP_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
