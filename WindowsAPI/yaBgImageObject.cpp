#include "yaBgImageObject.h"
#include "yaResources.h"
#include "yaImage.h"
#include "yaPlayer.h"
#include "yaRigidbody.h"

namespace ya
{
	BgImageObject::BgImageObject()
		: mImage(nullptr)
	{
		SetPos(Vector2::Zero);
		SetScale(Vector2::One);
	}

	BgImageObject::~BgImageObject()
	{

	}

	void BgImageObject::Initialize()
	{

	}

	void BgImageObject::Tick()
	{
		GameObject::Tick();

		//Playerpos;
		//mPixelImage->GetPixel(Playerpos);

		//if (mPlayer == nullptr)
		//	return;

		//Vector2 pos = mPlayer->GetPos();

		//Pixel pixel = mImage->GetPixelImage(pos.x, pos.y + 50.0f);

		//if (pixel.R == 255 && pixel.G == 0 && pixel.B == 255)
		//{
		//	mPlayer->GetComponent<Rigidbody>()->SetGround(true);
		//	Vector2 playerPos = mPlayer->GetPos();
		//	playerPos.y -= 1.0f;
		//	mPlayer->SetPos(playerPos);
		//}
		//else
		//{
		//	mPlayer->GetComponent<Rigidbody>()->SetGround(false);
		//}

	}

	void BgImageObject::Render(HDC hdc)
	{
		Vector2 pos = GetPos();
		Vector2 scale = GetScale();
		Vector2 finalPos = pos;

		Vector2 rect;
		rect.x = mImage->GetWidth() * scale.x;
		rect.y = mImage->GetHeight() * scale.y;

		TransparentBlt(hdc
			, (int)finalPos.x
			, (int)finalPos.y
			, (int)rect.x
			, (int)rect.y
			, mImage->GetDC(), 0, 0, mImage->GetWidth(), mImage->GetHeight()
			, RGB(255, 0, 255));

		GameObject::Render(hdc);
	}

	void BgImageObject::SetImage(const std::wstring& key, const std::wstring& fileName)
	{
		std::wstring path = L"..\\Resources\\Image\\";
		path += fileName;

		mImage = Resources::Load<Texture>(key, path);
	}
}