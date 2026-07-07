#pragma once

#include <string>

namespace Dawn
{
	// forward declarations
	class RawModel;

	namespace AssimpImporter
	{
		RawModel* LoadRawModel(const std::string& filename);
	}
}