#include "Image.h"
#include "Utils/Log.h"

#include <stb/stb_image.h>

namespace Dawn
{
	Image::Image(const std::string& filePath, int desiredChannels)
	{
		mFilePath = filePath;

		stbi_set_flip_vertically_on_load(true);
		mData = stbi_load(mFilePath.c_str(), &mWidth, &mHeight, &mNumChannels, desiredChannels);

		if (mData == nullptr)
			LOG_ERROR("Failed to load image: %s", filePath.c_str());
		else
			LOG_INFO("Loaded Image: %s", filePath.c_str());
	}

	Image::~Image()
	{
		if (mData)
		{
			stbi_image_free(mData);
			LOG_INFO("Destroyed Image: %s", mFilePath.c_str());
		}
	}
}