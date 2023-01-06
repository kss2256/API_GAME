#include "yaTitleScene.h"
#include "yaInput.h"
#include "yaSceneManager.h"
#include "yaBgImageObject.h"
#include "yaApplication.h"
#include "yaUIManager.h"

#include <Vfw.h>
#pragma comment (lib, "Vfw32.lib")

WindowData window;


HWND m_hVideo;

namespace ya
{
	TitleScene::TitleScene()
	{
		
		//window = Application::GetInstance().GetAtlasWindowData();

	

	}
	TitleScene::~TitleScene()
	{

	}
	void TitleScene::Initialize()
	{
		
	}
	void TitleScene::Tick()
	{
		Scene::Tick();
		

		if (MCIWndGetLength(m_hVideo) <= MCIWndGetPosition(m_hVideo))
		{
			Application::GetInstance().SetVideo(false);
			SceneManager::ChangeScene(eSceneType::Play);
			return;
		}


	}
	void TitleScene::Render(HDC hdc)
	{
		Scene::Render(hdc);
		//wchar_t szFloat[50] = {};
		//swprintf_s(szFloat, 50, L"Title Scene");
		//int strLen = (int)(wcsnlen_s(szFloat, 50));
		//TextOut(hdc, 10, 30, szFloat, strLen);
	}
	void TitleScene::Enter()
	{

		window = Application::GetInstance().GetWindowData();
		Application::GetInstance().SetVideo(true);
		m_hVideo = MCIWndCreate(window.hWnd, NULL, WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR
			, L"..\\Resources\\Video\\Opening.wmv");

		MoveWindow(m_hVideo, 0, 0, window.width, window.height, FALSE);

		MCIWndPlay(m_hVideo);

		Scene::Initialize();
		UIManager::Pop(eUIType::START);
		UIManager::Pop(eUIType::ANIMATIONPANEL);
		UIManager::Pop(eUIType::GROUNDBUTTON);
		UIManager::Pop(eUIType::HP);

	}
	void TitleScene::Exit()
	{
	}
}
