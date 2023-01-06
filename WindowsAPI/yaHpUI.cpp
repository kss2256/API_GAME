#include "yaHpUI.h"
#include "yaPlayer.h"
#include "yaImage.h"


namespace ya
{

	HpUI::HpUI(eUIType type)
		: UiBase(type)
	{
	}

	HpUI::~HpUI()
	{
	}

	void HpUI::OnInit()
	{
	}

	void HpUI::OnActive()
	{
	}

	void HpUI::OnInActive()
	{
	}

	void HpUI::OnTick()
	{
	}

	void HpUI::OnRender(HDC hdc)
	{
		if (mImage == nullptr)
			return;

		if (mTarget == nullptr)
			return;

		Player* playerObj = dynamic_cast<Player*>(mTarget);
		int hp = playerObj->GetHp();
		//float xRatio = (hp / 100.0f);

		BLENDFUNCTION func = {};
		func.BlendOp = AC_SRC_OVER;
		func.BlendFlags = 0;
		func.AlphaFormat = AC_SRC_ALPHA;
		func.SourceConstantAlpha = 255;

		AlphaBlend(hdc
			, (int)mScreenPos.x
			, (int)mScreenPos.y
			, mImage->GetWidth()
			, mImage->GetHeight()
			, mImage->GetDC(), 0, 0, mImage->GetWidth(), mImage->GetHeight(), func);


	}

	void HpUI::OnClear()
	{


	}



}