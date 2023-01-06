#include "yaImage.h"
#include "yaApplication.h"
#include "yaResources.h"

namespace ya
{
	Texture* Texture::Create(const std::wstring& key, UINT width, UINT height)
	{
		Texture* image = Resources::Find<Texture>(key);
		if (image != nullptr)
		{
			MessageBox(nullptr, L"�ߺ� Ű �̹��� ����", L"�̹��� ���� ����!", MB_OK);
			return nullptr;
		}

		// �̹����� ����������Ѵ�.
		image = new Texture();
		HDC mainHdc = Application::GetInstance().GetHdc();

		image->mBitmap = CreateCompatibleBitmap(mainHdc, width, height);
		image->mHdc = CreateCompatibleDC(mainHdc);

		if (image->mBitmap == NULL
			|| image->mHdc == NULL)
			return nullptr;

		// ���� ������ ��Ʈ�ʰ� DC �� ���� ����������մϴ�.
		HBITMAP defaultBitmap = (HBITMAP)SelectObject(image->mHdc, image->mBitmap);
		DeleteObject(defaultBitmap);

		//��Ʈ�� ���� Ȯ��
		BITMAP bitmap = {};
		GetObject(image->mBitmap, sizeof(BITMAP), &bitmap);

		image->mWidth = bitmap.bmWidth;
		image->mHeight = bitmap.bmHeight;

		image->SetKey(key);
		Resources::Insert<Texture>(key, image);

		return image;
	}
	Texture::Texture()
		: mBitmap(NULL)
		, mHdc(NULL)
		, mWidth(0)
		, mHeight(0)
	{
	}

	Texture::~Texture()
	{
		HWND hWnd = Application::GetInstance().GetWindowData().hWnd;
		ReleaseDC(hWnd, mHdc);
	}

	HRESULT Texture::Load(const std::wstring& path)
	{
		mBitmap = (HBITMAP)LoadImageW(nullptr, path.c_str()
			, IMAGE_BITMAP, 0, 0
			, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		if (mBitmap == nullptr)
		{
			return E_FAIL;
		}

		BITMAP bitInfo = {};
		GetObject(mBitmap, sizeof(BITMAP), &bitInfo);

		mWidth = bitInfo.bmWidth;
		mHeight = bitInfo.bmHeight;

		HDC mainDC = Application::GetInstance().GetWindowData().hdc;
		mHdc = CreateCompatibleDC(mainDC);

		HBITMAP prevBit = (HBITMAP)SelectObject(mHdc, mBitmap);
		DeleteObject(prevBit);

		return S_OK;
	}

	Pixel Texture::GetPixelImage(int x, int y)
	{

		COLORREF rgba = GetPixel(mHdc, x, y);

		return Pixel(rgba);
	}

	void Texture::SetPixel(int x, int y, Pixel pixel)
	{
		// ��Ʈ�� ��ǥ�� �����ϴ��� 0,0 
		// ��������ǥó�� ����Ϸ��� y�� �ݴ�� �ٲپ� �־�� �Ѵ�.
		y = mHeight - (y + 1);

		Pixel* bitmapPixel = (Pixel*)mBitmap;
		bitmapPixel += (mWidth * y + x);

		*bitmapPixel = pixel;
	}
}

