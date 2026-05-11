#include "Texture.h"
#include "Utils/Log.h"

#include <glad/glad.h>
#include <Rendering/Image.h>

namespace Dawn
{
	Texture::Texture(const Image* image)
	{
		if (!image || !image->IsValid())
		{
			LOG_ERROR("Tried to load an invalid image as texture");
			return;
		}

		if (!Load(image))
			LOG_ERROR("Failed to load Texture: %s", image->GetFilePath().c_str());
	}

	Texture::~Texture()
	{
		if (IsValid())
			glDeleteTextures(1, &mId);
	}

	bool Texture::Load(const Image* image)
	{
		mWidth = image->GetWidth();
		mHeight= image->GetHeight();
		mNumChannels = image->GetNumChannels();

		GLint internalFormat;
		GLenum format;
		if		(mNumChannels == 1) { format = GL_RED;	internalFormat = GL_R8; }
		else if (mNumChannels == 2) { format = GL_RG;	internalFormat = GL_RG8; }
		else if (mNumChannels == 3) { format = GL_RGB;	internalFormat = GL_RGB8; }
		else if (mNumChannels == 4) { format = GL_RGBA; internalFormat = GL_RGBA8; }
		else
		{
			LOG_ERROR("Unsupported channel count (%d) in image: %s", mNumChannels, image->GetFilePath().c_str());
			return false;
		}

		glGenTextures(1, &mId);
		glBindTexture(GL_TEXTURE_2D, mId);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, image->GetData());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glGenerateMipmap(GL_TEXTURE_2D);

		return true;
	}
	
	void Texture::Bind(int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, mId);
	}

}