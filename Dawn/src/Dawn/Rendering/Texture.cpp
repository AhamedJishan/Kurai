#include "Texture.h"
#include "Dawn/Utils/Log.h"

#include <glad/glad.h>
#include <Dawn/Rendering/Image.h>

namespace Dawn
{
	// --- OPENGL HELPERS ---
	GLint ToGlInternalFormat(TextureFormat textureFormat)
	{
		if (textureFormat == TextureFormat::R8) return GL_R8;
		if (textureFormat == TextureFormat::RG8) return GL_RG8;
		if (textureFormat == TextureFormat::RGB8) return GL_RGB8;
		if (textureFormat == TextureFormat::RGBA8) return GL_RGBA8;
		if (textureFormat == TextureFormat::R16F) return GL_R16F;
		if (textureFormat == TextureFormat::RG16F) return GL_RG16F;
		if (textureFormat == TextureFormat::RGB16F) return GL_RGB16F;
		if (textureFormat == TextureFormat::RGBA16F) return GL_RGBA16F;
		if (textureFormat == TextureFormat::RGBA32F) return GL_RGBA32F;
		if (textureFormat == TextureFormat::R11G11B10F) return GL_R11F_G11F_B10F;
		if (textureFormat == TextureFormat::Depth24) return GL_DEPTH_COMPONENT24;
		if (textureFormat == TextureFormat::Depth32F) return GL_DEPTH_COMPONENT32F;

		LOG_ERROR("Unknown TextureFormat");
		return GL_RGB8;
	}

	GLenum ToGlFormat(TextureFormat textureFormat)
	{
		if (textureFormat == TextureFormat::R8) return GL_RED;
		if (textureFormat == TextureFormat::RG8) return GL_RG;
		if (textureFormat == TextureFormat::RGB8) return GL_RGB;
		if (textureFormat == TextureFormat::RGBA8) return GL_RGBA;
		if (textureFormat == TextureFormat::R16F) return GL_RED;
		if (textureFormat == TextureFormat::RG16F) return GL_RG;
		if (textureFormat == TextureFormat::RGB16F) return GL_RGB;
		if (textureFormat == TextureFormat::RGBA16F) return GL_RGBA;
		if (textureFormat == TextureFormat::RGBA32F) return GL_RGBA;
		if (textureFormat == TextureFormat::R11G11B10F) return GL_RGB;
		if (textureFormat == TextureFormat::Depth24) return GL_DEPTH_COMPONENT;
		if (textureFormat == TextureFormat::Depth32F) return GL_DEPTH_COMPONENT;

		LOG_ERROR("Unknown TextureFormat");
		return GL_RGB;
	}

	GLenum ToGlType(TextureFormat textureFormat)
	{
		if (textureFormat == TextureFormat::R8) return GL_UNSIGNED_BYTE;
		if (textureFormat == TextureFormat::RG8) return GL_UNSIGNED_BYTE;
		if (textureFormat == TextureFormat::RGB8) return GL_UNSIGNED_BYTE;
		if (textureFormat == TextureFormat::RGBA8) return GL_UNSIGNED_BYTE;
		if (textureFormat == TextureFormat::R16F) return GL_FLOAT;
		if (textureFormat == TextureFormat::RG16F) return GL_FLOAT;
		if (textureFormat == TextureFormat::RGB16F) return GL_FLOAT;
		if (textureFormat == TextureFormat::RGBA16F) return GL_FLOAT;
		if (textureFormat == TextureFormat::RGBA32F) return GL_FLOAT;
		if (textureFormat == TextureFormat::R11G11B10F) return GL_FLOAT;
		if (textureFormat == TextureFormat::Depth24) return GL_UNSIGNED_INT;
		if (textureFormat == TextureFormat::Depth32F) return GL_FLOAT;

		LOG_ERROR("Unknown TextureFormat");
		return GL_UNSIGNED_BYTE;
	}
	// ----------------------



	Texture::Texture(const Image* image)
	{
		mId = 0;
		mIsImageTexture = true;

		if (!image || !image->IsValid())
		{
			LOG_ERROR("Tried to load an invalid image as texture");
			return;
		}

		if (!Load(image))
			LOG_ERROR("Failed to load Texture: %s", image->GetFilePath().c_str());
	}

	Texture::Texture(int width, int height, TextureFormat textureFormat)
	{
		mId = 0;
		mTextureFormat = textureFormat;
		mIsImageTexture = false;
		mWidth = width;
		mHeight = height;

		glGenTextures(1, &mId);
		glBindTexture(GL_TEXTURE_2D, mId);

		glTexImage2D(GL_TEXTURE_2D, 0, ToGlInternalFormat(mTextureFormat), mWidth, mHeight, 0, ToGlFormat(mTextureFormat), ToGlType(mTextureFormat), nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::~Texture()
	{
		if (IsValid())
			glDeleteTextures(1, &mId);
	}

	void Texture::Resize(const glm::vec2& size)
	{
		if (mIsImageTexture)
		{
			LOG_WARN("Can not resize image based textures... Yet!");
			return;
		}

		mWidth = size.x;
		mHeight = size.y;

		glBindTexture(GL_TEXTURE_2D, mId);
		glTexImage2D(GL_TEXTURE_2D, 0, ToGlInternalFormat(mTextureFormat), mWidth, mHeight, 0, ToGlFormat(mTextureFormat), ToGlType(mTextureFormat), nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	bool Texture::Load(const Image* image)
	{
		mWidth = image->GetWidth();
		mHeight= image->GetHeight();
		mNumChannels = image->GetNumChannels();

		if		(mNumChannels == 1) { mTextureFormat = TextureFormat::R8;}
		else if (mNumChannels == 2) { mTextureFormat = TextureFormat::RG8;}
		else if (mNumChannels == 3) { mTextureFormat = TextureFormat::RGB8;}
		else if (mNumChannels == 4) { mTextureFormat = TextureFormat::RGBA8;}
		else
		{
			LOG_ERROR("Unsupported channel count (%d) in image: %s", mNumChannels, image->GetFilePath().c_str());
			return false;
		}

		glGenTextures(1, &mId);
		glBindTexture(GL_TEXTURE_2D, mId);

		glTexImage2D(GL_TEXTURE_2D, 0, ToGlInternalFormat(mTextureFormat), mWidth, mHeight, 0, ToGlFormat(mTextureFormat), ToGlType(mTextureFormat), image->GetData());
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