#include "yaLife.h"
#include "yaImage.h"
#include "yaGameObject.h"
#include "yaPlayer.h"

#define Hp_Size 53;

namespace ya
{

	Life::Life(eUIType type)
		: UiBase(type)
	{

	}

	Life::~Life()
	{

	}


	void Life::OnRender(HDC hdc)
	{
		if (mImage == nullptr)
			return;

		if (mTarget == nullptr)
			return;

		Player* playerObj = dynamic_cast<Player*>(mTarget);
		int hp = playerObj->GetHp() * Hp_Size;
		

		BLENDFUNCTION func = {};
		func.BlendOp = AC_SRC_OVER;
		func.BlendFlags = 0;
		func.AlphaFormat = AC_SRC_ALPHA;
		func.SourceConstantAlpha = 255;

		AlphaBlend(hdc
			, (int)mScreenPos.x
			, (int)mScreenPos.y
			, hp//mImage->GetWidth()
			, mImage->GetHeight()
			, mImage->GetDC(), 0, 0
			, hp//mImage->GetWidth()
			, mImage->GetHeight(), func);

	}

}