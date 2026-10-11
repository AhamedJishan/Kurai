#pragma once
#include <filesystem>

namespace Dawn::File
{
	bool CreateEmpty(const std::filesystem::path& atPath, std::string stemName = "", std::string extension = "");
}