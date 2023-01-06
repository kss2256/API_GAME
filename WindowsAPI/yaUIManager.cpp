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
		// ���⿡�� ui �޸𸮿� �Ҵ��ϸ� �ȴ�.

		//Logo UI�� ��ư �Լ�
		LogoInitialize();

		//Ground UI�� ��ư �Լ�
		GroundInitialize();

		//Animation UI�� ��ư �Լ�
		AnimationInitialize();

		//Player UI �Լ�
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
			//UI �ε� ����
			eUIType requestUI = mRequestUIQueue.front();
			mRequestUIQueue.pop();

			OnLoad(requestUI);
		}
	}


	void UIManager::Render(HDC hdc)
	{
		std::stack<UiBase*> uiBases = mUIBases;
		std::stack<UiBase*> tempStack;

		// ����� �������� ���ش�.
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

		// ��ϸ��̼� ���� �̸� ����
		UINT pos = (UINT)m_AniPath.find_last_of(L"\\");
		std::wstring tail = m_AniPath.substr(pos + 1, m_AniPath.length()); //���ϸ� �̸�
		m_AniPath = m_AniPath.substr(0, pos);	//���ϸ��� ������ ��ü ���


		// �ִϸ��̼� ������Ʈ �̸� ����
		pos = (UINT)m_AniPath.find_last_of(L"\\");
		std::wstring head = m_AniPath.substr(pos + 1, m_AniPath.length());
		m_AniPath = head + tail;	//������ ��� + ���ϸ�

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

			// pop�ϴ� ui�� ��üȭ�� �ϰ�쿡,
			// ���� ui�߿� ��üȭ���� ���� ����� ui �� Ȱ��ȭ ���־���Ѵ�.
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



		//Logo UI�� ��ư
		Button* panelUI = new Button(eUIType::START);
		mUIs.insert(std::make_pair(eUIType::START, panelUI));
		panelUI->SetPos(Vector2(835.f, 580.f));
		panelUI->ImageLoad(L"Logo", L"..\\Resources\\Image\\Logo.bmp");

	}

	void UIManager::GroundInitialize()
	{

		//Ground UI�� �г�
		Panel* Groundpanel = new Panel(eUIType::GROUNDBUTTON);
		mUIs.insert(std::make_pair(eUIType::GROUNDBUTTON, Groundpanel));
		Groundpanel->SetPos(Vector2(1380.f, 10.f));
		Groundpanel->ImageLoad(L"OakUI", L"..\\Resources\\Image\\OakUI.bmp");

		//Ground Save ��ư
		SaveButton* GroundSave = new SaveButton(eUIType::SAVEBUTTON);
		mUIs.insert(std::make_pair(eUIType::SAVEBUTTON, GroundSave));
		GroundSave->SetPos(Vector2(30.f, 30.f));
		GroundSave->ImageLoad(L"button", L"..\\Resources\\Image\\button.bmp");

		//Ground Load ��ư
		LoadButton* GroundLoad = new LoadButton(eUIType::LOADBUTTON);
		mUIs.insert(std::make_pair(eUIType::LOADBUTTON, GroundLoad));
		GroundLoad->SetPos(Vector2(200.f, 30.f));
		GroundLoad->ImageLoad(L"button", L"..\\Resources\\Image\\button.bmp");

		//Ground Object1 ��ư
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
		// �޴� ������ ���� �м��մϴ�:
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

		//// ���� ������Ʈ
		// ȭ�� �����

		//DeleteObject();

		// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
		EndPaint(hWnd, &ps);
		//����
		//HFONT

		//HBITMAP
		//HBITMAP
		//DC ����

		// 1. PEN BRUSH �ڵ��� �����Ѵ�.
		// 2. GDI ������Ʈ�� �������ش�.
		// 3. ������ ������Ʈ�� hdc ����������Ѵ�. selectobject
		//����ϰ�

		// ������ ������Ʈ�� �ǵ����� ( ���� )
		// �ڵ��� �����Ѵ�.
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


