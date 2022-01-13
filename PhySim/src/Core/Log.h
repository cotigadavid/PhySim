#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace PhySim {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};

}

// Core log macros
#define PS_INFO(...)     ::PhySim::Log::GetLogger()->info(__VA_ARGS__)
#define PS_TRACE(...)    ::PhySim::Log::GetLogger()->trace(__VA_ARGS__)
#define PS_WARN(...)     ::PhySim::Log::GetLogger()->warn(__VA_ARGS__)
#define PS_ERROR(...)    ::PhySim::Log::GetLogger()->error(__VA_ARGS__)
#define PS_FATAL(...)    ::PhySim::Log::GetLogger()->fatal(__VA_ARGS__)