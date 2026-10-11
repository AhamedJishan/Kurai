#include "FileManager.h"

#include <string>
#include <regex>
#include <fstream>
#include <Dawn/Utils/Log.h>

namespace Dawn::File
{
	static const std::string UNTITLED = "untitled";

	bool CreateEmpty(const std::filesystem::path& atPath, std::string stemName, std::string extension)
	{
		int suffix = 0;
		std::string filename = stemName.empty() ? UNTITLED : stemName + (extension.empty() ? "" : "." + extension);
		std::filesystem::path filepath = atPath / filename;

		while (std::filesystem::exists(filepath))
		{
			if (stemName.empty())
				filepath = atPath / (UNTITLED + " (" + std::to_string(++suffix) + ")");
			else
				filepath = atPath / (stemName + " (" + std::to_string(++suffix) + ")" + (extension.empty() ? "" : "." + extension));
		}

		std::ofstream file(filepath);
		if (!file.is_open())
		{
			LOG_ERROR("Failed to create file '%s'", filepath.string().c_str());
			return false;
		}
	}
}