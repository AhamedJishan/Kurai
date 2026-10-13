#pragma once
#include <filesystem>

namespace Dawn::FileSystem
{
	// extension should include the dot '.'
	bool CreateEmpty(const std::filesystem::path& atPath, std::string stemName = "", std::string extension = "");
	bool RenameFile(const std::filesystem::path& filepath, const std::string& newName);
	bool IsValidFileName(const std::string& filename);
}