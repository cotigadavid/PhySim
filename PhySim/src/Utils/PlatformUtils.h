#pragma once

#include <string>

namespace PhySim {

	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

}