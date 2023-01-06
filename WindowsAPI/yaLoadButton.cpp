#include "yaLoadButton.h"
#include "yaEarth.h"
#include "yaImage.h"
#include "yaSceneManager.h"
#include "yaScene.h"


#include "yaBaldur.h"
#include "yaObject.h"


namespace ya
{
	LoadButton::LoadButton(eUIType type)
		: Button(type)
	{
	}

	LoadButton::~LoadButton()
	{

	}



	void LoadButton::Click()
	{

		FILE* file = nullptr;
		Earth* LoadBuuton = new Earth;
		LoadBuuton->Load(file);

		//Scene* playScene = SceneManager::GetPlayScene();
		//playScene->AddGameObject(LoadBuuton, eColliderLayer::Ground);


		Scene* playScene = SceneManager::GetPlayScene();
		playScene->AddGameObject(LoadBuuton, eColliderLayer::Monster);

	}

	void LoadButton::MouseDown()
	{
		

	}

	void LoadButton::MouseOn()
	{

	}

	void LoadButton::OnRender(HDC hdc)
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

