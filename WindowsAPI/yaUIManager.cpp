#include "Common.h"
#include "yaUIManager.h"
#include "yaHUD.h"
#include "yaHpUI.h"
#include "yaButton.h"
#include "yaPanel.h"
#include "yaLife.h"
#include "yaSaveButton.h"
#include "yaLoadButton.h"
#include "yaGroundButton.h"
#include "yaAnimationBtuuon.h"
#include "yaInput.h"
#include "WindowsAPI.h"
#include <commdlg.h>



namespace ya
{
	std::unordered_map<eUIType, UiBase*> UIManager::mUIs;
	std::queue<eUIType> UIManager::mRequestUIQueue;
	std::stack<UiBase*> UIManager::mUIBases;
	UiBase* UIManager::mCurrentData = nullptr;
	AnimationBtuuon* UIManager::m_Cheak = nullptr;

	void UIManager::Initialize()
	{
		// 여기에서 ui 메모리에 할당하면 된다.

		//Logo UI용 버튼 함수
		LogoInitialize();

		//Ground UI용 버튼 함수
		GroundInitialize();

		//Animation UI용 버튼 함수
		AnimationInitialize();

		//Player UI 함수
		PlayInitialize();

	}

	void UIManager::OnLoad(eUIType type)
	{
		std::unordered_map<eUIType, UiBase*>::iterator iter = mUIs.find(type);
		if (iter == mUIs.end())
		{
			OnFail();
			return;
		}

		OnComplete(iter->second);
	}

	void UIManager::Tick()
	{
		std::stack<UiBase*> uiBases = mUIBases;
		while (!uiBases.empty())
		{
			UiBase* uiBase = uiBases.top();
			if (uiBase != nullptr)
			{
				uiBase->Tick();
			}
			uiBases.pop();
		}

		if (mRequestUIQueue.size() > 0)
		{
			//UI 로드 해줘
			eUIType requestUI = mRequestUIQueue.front();
			mRequestUIQueue.pop();

			OnLoad(requestUI);
		}
	}


	void UIManager::Render(HDC hdc)
	{
		std::stack<UiBase*> uiBases = mUIBases;
		std::stack<UiBase*> tempStack;

		// 뒤집어서 렌더링을 해준다.
		while (!uiBases.empty())
		{
			UiBase* uiBase = uiBases.top();
			tempStack.push(uiBase);
			uiBases.pop();
		}

		while (!tempStack.empty())
		{
			UiBase* uiBase = tempStack.top();
			if (uiBase != nullptr)
			{
				uiBase->Render(hdc);
			}
			tempStack.pop();
		}
	}

	void UIManager::OnComplete(UiBase* addUI)
	{
		if (addUI == nullptr)
			return;

		addUI->Initialize();
		addUI->Active();
		addUI->Tick();

		if (addUI->GetIsFullScreen())
		{
			std::stack<UiBase*> uiBases = mUIBases;
			while (!uiBases.empty())
			{
				UiBase* uiBase = uiBases.top();
				uiBases.pop();

				if (uiBase->GetIsFullScreen())
				{
					uiBase->InActive();
				}
			}
		}

		mUIBases.push(addUI);
	}

	void UIManager::OnFail()
	{
		mCurrentData = nullptr;
	}

	void UIManager::Release()
	{
		for (auto ui : mUIs)
		{
			delete ui.second;
			ui.second = nullptr;
		}
	}

	void UIManager::AnimationImage()
	{

		OPENFILENAME ofn = {};

		std::wstring PATH = L"..\\Resources\\Image\\";

		wchar_t szFilePath[256] = {};

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFilePath;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = L"0ALL\0*.*";//L"Tile\0*.tile\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = PATH.c_str();//NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (false == GetOpenFileName(&ofn))
			return;

		std::wstring m_AniPath = szFilePath;

		// 얘니메이션 폴더 이름 추출
		UINT pos = (UINT)m_AniPath.find_last_of(L"\\");
		std::wstring tail = m_AniPath.substr(pos + 1, m_AniPath.length()); //파일명 이름
		m_AniPath = m_AniPath.substr(0, pos);	//파일명을 제외한 전체 경로


		// 애니메이션 오브젝트 이름 추출
		pos = (UINT)m_AniPath.find_last_of(L"\\");
		std::wstring head = m_AniPath.substr(pos + 1, m_AniPath.length());
		m_AniPath = head + tail;	//마지막 경로 + 파일명

		m_Cheak->SetPos(Vector2(0.f, 0.f));
		m_Cheak->ImageLoad(tail, szFilePath);

	}

	void UIManager::Push(eUIType type)
	{
		mRequestUIQueue.push(type);
	}

	void UIManager::Pop(eUIType type)
	{
		if (mUIBases.size() <= 0)
			return;

		std::stack<UiBase*> tempStack;

		UiBase* uiBase = nullptr;
		while (mUIBases.size() > 0)
		{
			uiBase = mUIBases.top();
			mUIBases.pop();

			// pop하는 ui가 전체화면 일경우에,
			// 남은 ui중에 전체화면인 가장 상단의 ui 를 활성화 해주어야한다.
			if (uiBase->GetType() == type)
			{
				if (uiBase->GetIsFullScreen())
				{
					std::stack<UiBase*> uiBases = mUIBases;
					while (!uiBases.empty())
					{
						UiBase* uiBase = uiBases.top();
						uiBases.pop();

						if (uiBase->GetIsFullScreen())
						{
							uiBase->Active();
							break;
						}
					}
				}
				uiBase->InActive();
				uiBase->UIClear();
			}
			else
			{
				tempStack.push(uiBase);
			}
		}

		while (tempStack.size())
		{
			uiBase = tempStack.top();
			tempStack.pop();
			mUIBases.push(uiBase);
		}

	}

	void UIManager::LogoInitialize()
	{



		//Logo UI용 버튼
		Button* panelUI = new Button(eUIType::START);
		mUIs.insert(std::make_pair(eUIType::START, panelUI));
		panelUI->SetPos(Vector2(835.f, 580.f));
		panelUI->ImageLoad(L"Logo", L"..\\Resources\\Image\\Logo.bmp");

	}

	void UIManager::GroundInitialize()
	{

		//Ground UI용 패널
		Panel* Groundpanel = new Panel(eUIType::GROUNDBUTTON);
		mUIs.insert(std::make_pair(eUIType::GROUNDBUTTON, Groundpanel));
		Groundpanel->SetPos(Vector2(1380.f, 10.f));
		Groundpanel->ImageLoad(L"OakUI", L"..\\Resources\\Image\\OakUI.bmp");

		//Ground Save 버튼
		SaveButton* GroundSave = new SaveButton(eUIType::SAVEBUTTON);
		mUIs.insert(std::make_pair(eUIType::SAVEBUTTON, GroundSave));
		GroundSave->SetPos(Vector2(30.f, 30.f));
		GroundSave->ImageLoad(L"button", L"..\\Resources\\Image\\button.bmp");

		//Ground Load 버튼
		LoadButton* GroundLoad = new LoadButton(eUIType::LOADBUTTON);
		mUIs.insert(std::make_pair(eUIType::LOADBUTTON, GroundLoad));
		GroundLoad->SetPos(Vector2(200.f, 30.f));
		GroundLoad->ImageLoad(L"button", L"..\\Resources\\Image\\button.bmp");

		//Ground Object1 버튼
		GroundButton* GroundObject1 = new GroundButton(eUIType::GROUND_OBJECT);
		mUIs.insert(std::make_pair(eUIType::GROUND_OBJECT, GroundObject1));
		GroundObject1->SetPos(Vector2(30.f, 300.f));
		GroundObject1->ImageLoad(L"GroundTool", L"..\\Resources\\Image\\GroundTool.bmp");



		Groundpanel->AddChild(GroundSave);
		Groundpanel->AddChild(GroundLoad);
		Groundpanel->AddChild(GroundObject1);
	}

	void UIManager::AnimationInitialize()
	{
		m_Cheak = new AnimationBtuuon(eUIType::ANIMATIONPANEL);
		mUIs.insert(std::make_pair(eUIType::ANIMATIONPANEL, m_Cheak));
	}

	void UIManager::PlayInitialize()
	{
		HpUI* hp = new HpUI(eUIType::HP);
		mUIs.insert(std::make_pair(eUIType::HP, hp));
		hp->SetPos(Vector2(20.f, 20.f));
		hp->ImageLoad(L"Hp", L"..\\Resources\\Image\\Hp.bmp");

		Life* life = new Life(eUIType::LIFE);
		mUIs.insert(std::make_pair(eUIType::LIFE, life));
		life->SetPos(Vector2(130.f, 65.f));
		life->ImageLoad(L"Life", L"..\\Resources\\Image\\Life.bmp");

		hp->AddChild(life);

	}

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{

	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		
		case ID_TILE_SAVE:
			
		{
			int a = 0;
		}
		break;

		case ID_TILE_LOAD:
			
		{
			ya::UIManager::AnimationImage();		
		}
		break;


		case IDM_EXIT:
			DestroyWindow(hWnd);
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_KEYDOWN:
	{


	}
	break;

	case WM_TIMER:
	{

	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		//// 스톡 오브젝트
		// 화면 지우기

		//DeleteObject();

		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		EndPaint(hWnd, &ps);
		//문자
		//HFONT

		//HBITMAP
		//HBITMAP
		//DC 정리

		// 1. PEN BRUSH 핸들을 선언한다.
		// 2. GDI 오브젝트를 생성해준다.
		// 3. 생성된 오브젝트로 hdc 세팅해줘야한다. selectobject
		//사용하고

		// 기존의 오브젝트로 되돌린다 ( 해제 )
		// 핸들을 삭제한다.
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		//KillTimer(hWnd, 0);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


