#pragma once

namespace Dawn
{
	enum class TextureFormat
	{
		R8,
		RG8,
		RGB8,
		RGBA8,

		R16F,
		RG16F,
		RGB16F,
		RGBA16F,
		RGBA32F,

		R11G11B10F,

		Depth24,
		Depth32F
	};

	class Texture
	{
	public:
		Texture(const class Image* image);
		Texture(int width, int height, TextureFormat textureFormat);
		~Texture();

		void Bind(int slot = 0);

		bool IsValid() const { return mId != 0; }
		unsigned int GetId() const { return mId; }
		int GetWidth() const { return mWidth; }
		int GetHeight() const { return mHeight; }

		void SetSize(int width, int height);

	private:
		bool Load(const class Image* image);

	private:
		unsigned int mId = 0;

		int mWidth = 0;
		int mHeight = 0;
		int mNumChannels = 0;
		bool mIsImageTexture;
		TextureFormat mTextureFormat;
	};
}