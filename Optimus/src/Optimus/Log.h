#pragma once
#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace OP {

	class OPTIMUS_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;  }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//Core Loggers
#define OP_CORE_TRACE(...)		::OP::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define OP_CORE_INFO(...)		::OP::Log::GetCoreLogger()->info(__VA_ARGS__)
#define OP_CORE_WARN(...)		::OP::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define OP_CORE_ERROR(...)		::OP::Log::GetCoreLogger()->error(__VA_ARGS__)
#define OP_CORE_FATAL(...)		::OP::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define OP_ASSERT(x) if (!(x)) {OP_CORE_ERROR("Assertion Failed! File: {0}, Line: {1}",__FILE__, __LINE__);}


//Client Loggers
#define OP_TRACE(...)			::OP::Log::GetClientLogger()->trace(__VA_ARGS__)
#define OP_INFO(...)			::OP::Log::GetClientLogger()->info(__VA_ARGS__)
#define OP_WARN(...)			::OP::Log::GetClientLogger()->warn(__VA_ARGS__)
#define OP_ERROR(...)			::OP::Log::GetClientLogger()->error(__VA_ARGS__)
#define OP_FATAL(...)			::OP::Log::GetClientLogger()->critical(__VA_ARGS__)
