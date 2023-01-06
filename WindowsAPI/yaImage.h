#pragma once
#include "yaResource.h"

namespace ya
{
	class Texture : public Resource
	{
	public:
		static Texture* Create(const std::wstring& key, UINT width, UINT height);

		Texture();
		~Texture();

		virtual HRESULT Load(const std::wstring& path) override;

		HDC GetDC() { return mHdc; }
		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }

		Pixel GetPixelImage(int x, int y);
		void SetPixel(int x, int y, Pixel pixel);

	private:
		HBITMAP mBitmap;
		HDC mHdc;
		UINT mWidth;
		UINT mHeight;
	};
}
