#include "yaSaveButton.h"
#include "yaEarth.h"
#include "yaImage.h"
#include "yaSceneManager.h"
#include "yaScene.h"


namespace ya
{

	SaveButton::SaveButton(eUIType type)
		: Button(type)

	{

	}
	SaveButton::~SaveButton()
	{

	}


	void SaveButton::Click()
	{
		FILE* file = nullptr;
		Earth* SaveBuuton = new Earth;
		//SaveBuuton->Save(file);
		//
		//
		//Scene* playScene = SceneManager::GetPlayScene();
		//playScene->AddGameObject(SaveBuuton, eColliderLayer::Ground);

		SaveBuuton->Save(file);


		Scene* playScene = SceneManager::GetPlayScene();
		playScene->AddGameObject(SaveBuuton, eColliderLayer::Monster);

		

	}

	void SaveButton::MouseDown()
	{
	}

	void SaveButton::MouseOn()
	{


	}

	void SaveButton::OnRender(HDC hdc)
	{


		BLENDFUNCTION func = {};
		func.BlendOp = AC_SRC_OVER;
		func.BlendFlags = 0;
		func.AlphaFormat = AC_SRC_ALPHA;
		func.SourceConstantAlpha = 255;

		if (nullptr != mImage)
		{
			AlphaBlend(hdc, (int)mScreenPos.x, (int)mScreenPos.y
				, mImage->GetWidth(), mImage->GetHeight()
				, mImage->GetDC(), 0, 0, mImage->GetWidth(), mImage->GetHeight(), func);
		}



	}






}