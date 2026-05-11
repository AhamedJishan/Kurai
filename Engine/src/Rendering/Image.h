#pragma once

#include <string>

namespace Dawn
{
	/// Owns raw image data.
	/// Loads pixels on construction and frees them automatically on destruction.
	/// IsValid() reports whether loading succeeded.
	class Image
	{
	public:
		Image(const std::string& filePath, int desiredChannels = 0);
		~Image();

		// Delete copy and move
		Image(const Image&) = delete;
		Image& operator=(const Image&) = delete;

		bool IsValid() const { return mData != nullptr; }
		unsigned char* GetData() const { return mData; }

		const std::string& GetFilePath() const { return mFilePath; }

		int GetWidth() const { return mWidth; }
		int GetHeight() const { return mHeight; }
		int GetNumChannels() const { return mNumChannels; }

	private:
		unsigned char* mData = nullptr;
		std::string mFilePath;

		int mWidth = 0;
		int mHeight = 0;
		int mNumChannels = 0;
	};
}