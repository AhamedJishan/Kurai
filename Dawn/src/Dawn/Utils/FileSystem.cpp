#include "FileSystem.h"

#include <string>
#include <fstream>
#include <Dawn/Utils/Log.h>

namespace Dawn::FileSystem
{
	static const std::string UNTITLED = "untitled";

	bool CreateEmpty(const std::filesystem::path& atPath, std::string stemName, std::string extension)
	{
		int suffix = 0;
		std::string filename = stemName.empty() ? UNTITLED : stemName + (extension.empty() ? "" : extension);
		std::filesystem::path filepath = atPath / filename;

		while (std::filesystem::exists(filepath))
		{
			if (stemName.empty())
				filepath = atPath / (UNTITLED + " (" + std::to_string(++suffix) + ")");
			else
				filepath = atPath / (stemName + " (" + std::to_string(++suffix) + ")" + (extension.empty() ? "" : extension));
		}

		std::ofstream file(filepath);
		if (!file.is_open())
		{
			LOG_ERROR("Failed to create file '%s'", filepath.string().c_str());
			return false;
		}

		return true;
	}

	bool RenameFile(const std::filesystem::path& filepath, const std::string& newName)
	{
		std::filesystem::path newPath = filepath.parent_path() / newName;

		std::error_code ec;
		if (std::filesystem::exists(newPath, ec))
		{
			LOG_ERROR("'%s' already exists", filepath.string().c_str());
			return false;
		}

		std::filesystem::rename(filepath, newPath, ec);		
		if (ec)
		{
			LOG_ERROR("Failed to rename file '%s': %s", filepath.string().c_str(), ec.message().c_str());
			return false;
		}

		return true;
	}

	bool IsValidFileName(const std::string& filename)
	{
		if (filename.empty())
		{
			LOG_WARN("File name can't be empty");
			return false;
		}
		if (filename.find_first_of("\\/:*?\"<>|") != std::string::npos)
		{
			LOG_WARN("Filename '%s' has illegal characters '\\/:*?\"<>|'", filename.c_str());
			return false;
		}
		if (filename.back() == ' ' || filename.back() == '.')
		{
			LOG_WARN("Filename can't end with a SPACE or '.'");
			return false;
		}

		return true;
	}
}