#pragma once

/* IMPLEMENT PER PLATFORM! */

namespace Above
{
	class FileDialogs
	{
	public:
		// Returns empty string if cancelled
		static std::string OpenFile(const char* filter);

		// Returns empty string if cancelled
		static std::string SaveFile(const char* filter);
	};
}