#include "yaLogoScene.h"
#include "yaPlayer.h"
#include "yaMonster.h"
#include "yaInput.h"
#include "yaSceneManager.h"
#include "yaBgImageObject.h"
#include "yaCollisionManager.h"
#include "yaObject.h"
#include "yaResources.h"
#include "yaApplication.h"
#include "yaGroundTool.h"
#include "yaGround.h"
#include "yaUIManager.h"
#include "yaHUD.h"
#include "yaPanel.h"
#include "yaButton.h"
#include "yaTime.h"
#include "yaCamera.h"
#include "yaImage.h"
#include "yaSoundManager.h"
#include "yaSound.h"

#include "yaInput.h"

namespace ya
{



	LogoScene::LogoScene()
		: m_eMode(eModeType::None)
		, mImage(nullptr)
		, mCheak(nullptr)
		, mSound(nullptr)
		, mSound1(nullptr)
		, m_fChange(0.f)
		, m_bChange(false)
	
	{
		if(mImage == nullptr)
		mImage = Resources::Load<Texture>(L"Start", L"..\\Resources\\Image\\Start.bmp");
		
		if (mSound == nullptr)
		mSound = Resources::Load<Sound>(L"Logo", L"..\\Resources\\Sound\\Logo.wav");
		

	

		
		

	}


	LogoScene::~LogoScene()
	{
	
		if (mSound != nullptr)
			delete mSound;

	}

	void LogoScene::Initialize()
	{




		




	}

	void LogoScene::Tick()
	{
		// 오브젝트 tick 을 호출한다.
		Scene::Tick();


	




		

		
	


		if (m_bChange)
		{
			
			m_fChange += DT;
			if (m_fChange > 3.f)
			{
				
				Camera::SetCameraEffect(eCameraEffect::FadeIn);
				switch (m_eMode)
				{
				case eModeType::Ground:
				{			
					m_bChange = false;
					ya::SceneManager::ChangeScene(eSceneType::Ground_Tool);
				}
					break;
				case eModeType::Animation:
					m_bChange = false;
					ya::SceneManager::ChangeScene(eSceneType::Animation_Tool);
					break;
				case eModeType::Play:
					m_bChange = false;
					ya::SceneManager::ChangeScene(eSceneType::Title);
					//ya::SceneManager::ChangeScene(eSceneType::Play);
					break;
				case eModeType::Exit:

					break;
				}
			}
		}


	}

	void LogoScene::Render(HDC hdc)
	{
		Scene::Render(hdc);
		//
		wchar_t szFloat[50] = {};
		swprintf_s(szFloat, 50, L"Logo Scene.bmp");
		int strLen = (int)(wcsnlen_s(szFloat, 50));
		TextOut(hdc, 10, 30, szFloat, strLen);



		TransparentBlt(hdc
			, 0
			, 0
			, mImage->GetWidth(), mImage->GetHeight()
			, mImage->GetDC(), 0, 0, mImage->GetWidth(), mImage->GetHeight(), RGB(255, 0, 255));





	}

	void LogoScene::Enter()
	{
		Scene::Initialize();

		//if (mSound)
		//	mSound->Play(true);
	
		UIManager::Pop(eUIType::GROUNDBUTTON);
		UIManager::Pop(eUIType::ANIMATIONPANEL);
		UIManager::Pop(eUIType::HP);
		UIManager::Push(eUIType::START);
		
		
		
	}

	void LogoScene::Exit()
	{
		
	

	}


	void LogoScene::Start()
	{
		
		Camera::SetCameraEffect(eCameraEffect::FadeOut);
		m_eMode = eModeType::Play;
		m_bChange = true;
	}
	void LogoScene::Ground()
	{
	
		Camera::SetCameraEffect(eCameraEffect::FadeOut);
	
		m_eMode = eModeType::Ground;
		m_bChange = true;
	}
	void LogoScene::Animation()
	{
		
		Camera::SetCameraEffect(eCameraEffect::FadeOut);

		m_eMode = eModeType::Animation;
		m_bChange = true;

	}
}