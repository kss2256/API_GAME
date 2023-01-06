#include "yaAnimationBtuuon.h"
#include "yaApplication.h"
#include "yaImage.h"
#include "yaSceneManager.h"
#include "yaCamera.h"
#include "yaResources.h"
#include "yaInput.h"
#include "yaAnimator.h"
#include "yaAniPreview.h"
#include "yaPlayer.h"
#include "yaAnimation.h"
#include "yaObject.h"
#include "yaRigidbody.h"
#include "yaComponent.h"

#include <commdlg.h>


namespace ya
{

	float AnimationBtuuon::m_fDuration = 0.f;

	AnimationBtuuon::AnimationBtuuon(eUIType type)
		: Button(type)
		, m_pImg(nullptr)
		, m_pSaveImg(nullptr)
		, m_pLoadImg(nullptr)
		, m_pmator(nullptr)
		, m_vecfrm()
		, m_vDownPos(Vector2::Zero)
		, m_vClickedPos(Vector2::Zero)
		, m_vDownrenderPos(Vector2::Zero)
		, m_vClickedrenderPos(Vector2::Zero)
		, m_FinalPos(Vector2::Zero)
		, m_FinalScale(Vector2::Zero)
		, m_vButtonPos(Vector2::Zero)
	{
		m_pImg = Resources::Load<Texture>(L"AnimationUI", L"..\\Resources\\Image\\AnimationUI.bmp");
		m_pSaveImg = Resources::Load<Texture>(L"button", L"..\\Resources\\Image\\button.bmp");
		m_pLoadImg = Resources::Load<Texture>(L"button", L"..\\Resources\\Image\\button.bmp");


		if(m_pmator == nullptr)
		m_pmator = new Animator();
		
	}

	AnimationBtuuon::~AnimationBtuuon()
	{
		if (nullptr != m_pmator)
		{
			delete m_pmator;
			m_pmator = nullptr;
		}
	}


	void AnimationBtuuon::OnTick()
	{
		Button::OnTick();

		if (KEY_DOWN(eKeyCode::NUM_4))
		{
			m_vDownrenderPos.x -= 1.f;
		}
		if (KEY_DOWN(eKeyCode::NUM_6))
		{
			m_vDownrenderPos.x += 1.f;
		}
		if (KEY_DOWN(eKeyCode::NUM_8))
		{
			m_vDownrenderPos.y -= 1.f;
		}
		if (KEY_DOWN(eKeyCode::NUM_5))
		{
			m_vDownrenderPos.y += 1.f;
		}

		if (KEY_DOWN(eKeyCode::N_8))
		{
			m_fDuration += 0.1f;
		}
		if (KEY_DOWN(eKeyCode::N_9))
		{
			m_fDuration -= 0.1f;
		}


		if (KEY_DOWN(eKeyCode::LCTRL))
		{
			//전역함수로 듀레이션 지금은 삭제
			//float       m_fDuration;			

			Sprite frm = {};

			Vector2 Pos = {};
			Pos.x = m_vDownrenderPos.x;
			Pos.y = m_vDownrenderPos.y;
			Vector2 Scale = {};
			Scale.x = m_FinalScale.x;
			Scale.y = m_FinalScale.y;

			frm.leftTop = m_vDownrenderPos;
			frm.size = m_FinalScale;
			frm.duration = m_fDuration;

			m_vecfrm.push_back(frm);
		}
		if (KEY_DOWN(eKeyCode::LSHIFT))
		{	
		
			//m_pmator->PushAnimation(L"TaT", mImage, m_vecfrm);
			//m_pmator->SaveAnima(L"TaT", mImage, m_vecfrm);
		}

		if (KEY_DOWN(eKeyCode::ENTER))
		{			

		}

		m_vButtonPos = MOUSE_POS;
		if(KEY_DOWN(eKeyCode::LBTN))
		{
			if (m_vButtonPos.x >= 1500 && m_vButtonPos.y >= 60
				&& m_vButtonPos.x <= m_pSaveImg->GetWidth() + 1500 && m_vButtonPos.y <= m_pSaveImg->GetHeight() + 60)
			{
				SaveButton();

			}
			if (m_vButtonPos.x >= 1750 && m_vButtonPos.y >= 60
				&& m_vButtonPos.x <= m_pSaveImg->GetWidth() + 1750 && m_vButtonPos.y <= m_pSaveImg->GetHeight() + 60)
			{
				LoadButton();
			}
		}
		
		
	}

	void AnimationBtuuon::Click()
	{

		m_vClickedPos = MOUSE_POS;
		m_vClickedrenderPos = Camera::CalRealPos(MOUSE_POS);
		


		//중간 위치 정보
		m_FinalPos.x = (m_vDownPos.x + m_vClickedPos.x) / 2;
		m_FinalPos.y = (m_vDownPos.y + m_vClickedPos.y) / 2;

		//크기 정보
		m_FinalScale.x = abs(m_vDownPos.x - m_vClickedPos.x);
		m_FinalScale.y = abs(m_vDownPos.y - m_vClickedPos.y);


		

	}

	void AnimationBtuuon::MouseDown()
	{
		m_vDownPos = MOUSE_POS;

		m_vDownrenderPos = Camera::CalRealPos(MOUSE_POS);
	}

	void AnimationBtuuon::MouseOn()
	{



	}
	void AnimationBtuuon::OnRender(HDC hdc)
	{

		Vector2 Pos = Camera::CalculatePos(Vector2(0.f, 0.f));
		

		BLENDFUNCTION func = {};
		func.BlendOp = AC_SRC_OVER;
		func.BlendFlags = 0;
		func.AlphaFormat = AC_SRC_ALPHA;
		func.SourceConstantAlpha = 255;

		if (nullptr != mImage)
		{
			AlphaBlend(hdc
				, (int)Pos.x
				, (int)Pos.y
				, mImage->GetWidth(), mImage->GetHeight()
				, mImage->GetDC(), 0, 0, mImage->GetWidth(), mImage->GetHeight(), func);
		}

		if (nullptr != m_pImg)
		{
			AlphaBlend(hdc
				, 1200//(int)POS.x
				, 290//(int)POS.y
				, m_pImg->GetWidth(), m_pImg->GetHeight()
				, m_pImg->GetDC(), 0, 0, m_pImg->GetWidth(), m_pImg->GetHeight(), func);			
		}
		if (nullptr != m_pSaveImg)
		{
			AlphaBlend(hdc
				, 1500//(int)POS.x
				, 60//(int)POS.y
				, m_pSaveImg->GetWidth(), m_pSaveImg->GetHeight()
				, m_pSaveImg->GetDC(), 0, 0, m_pSaveImg->GetWidth(), m_pSaveImg->GetHeight(), func);
		}		
		if (nullptr != m_pLoadImg)
		{
			AlphaBlend(hdc
				, 1750//(int)POS.x
				, 60//(int)POS.y
				, m_pSaveImg->GetWidth(), m_pSaveImg->GetHeight()
				, m_pSaveImg->GetDC(), 0, 0, m_pSaveImg->GetWidth(), m_pSaveImg->GetHeight(), func);
		}
		//HPEN pen = (HPEN)Application::GetInstance().GetPen(ePenColor::Green);
		//Pen greenpen(hdc, pen);

		HPEN GreenPen = (HPEN)CreatePen(PS_SOLID ,1, RGB(0,255,0));
		HPEN OriginalPen = (HPEN)SelectObject(hdc, GreenPen);
		

		HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH hOriginBrush = (HBRUSH)SelectObject(hdc, hNullBrush);



		Rectangle(hdc
			, (int)(m_FinalPos.x - m_FinalScale.x / 2)
			, (int)(m_FinalPos.y - m_FinalScale.y / 2)
			, (int)(m_FinalPos.x + m_FinalScale.x / 2)
			, (int)(m_FinalPos.y + m_FinalScale.y / 2));

		GreenPen = (HPEN)SelectObject(hdc, OriginalPen);
		DeleteObject(GreenPen);
		SelectObject(hdc, hOriginBrush);		


		if(nullptr != mImage)
		{
			AlphaBlend(hdc
				, (int)(1550 - m_FinalScale.x / 2)
				, (int)(640 - m_FinalScale.y / 2)
				, (int)(m_FinalScale.x)
				, (int)(m_FinalScale.y)
				, mImage->GetDC()
				, (int)m_vDownrenderPos.x
				, (int)m_vDownrenderPos.y
				, (int)m_FinalScale.x
				, (int)m_FinalScale.y
				, func);
		}



		 
		

	}

	void AnimationBtuuon::SaveButton()
	{

		OPENFILENAME ofn = {};

		std::wstring PATH = L"..\\Resources\\AnimationSaveFiles\\";


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

		if (false == GetSaveFileName(&ofn))
			return;

		m_AniPath = szFilePath;

		// 얘니메이션 폴더 이름 추출
		UINT pos = (UINT)m_AniPath.find_last_of(L"\\");
		std::wstring tail = m_AniPath.substr(pos + 1, m_AniPath.length()); //파일명 이름
		m_AniPath = m_AniPath.substr(0, pos);	//파일명을 제외한 전체 경로


		// 애니메이션 오브젝트 이름 추출
		pos = (UINT)m_AniPath.find_last_of(L"\\");
		std::wstring head = m_AniPath.substr(pos + 1, m_AniPath.length());
		m_AniPath = head + tail;	//마지막 경로 + 파일명
		

		m_pmator->PushAnimation(tail, mImage, m_vecfrm);
		m_pmator->SaveAnima(tail, szFilePath, mImage, m_vecfrm);

	}

	void AnimationBtuuon::LoadButton()
	{
		OPENFILENAME ofn = {};

		std::wstring PATH = L"..\\Resources\\AnimationSaveFiles\\";

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

		m_AniPath = szFilePath;

		// 얘니메이션 폴더 이름 추출
		UINT pos = (UINT)m_AniPath.find_last_of(L"\\");
		std::wstring tail = m_AniPath.substr(pos + 1, m_AniPath.length()); //파일명 이름
		m_AniPath = m_AniPath.substr(0, pos);	//파일명을 제외한 전체 경로


		// 애니메이션 오브젝트 이름 추출
		pos = (UINT)m_AniPath.find_last_of(L"\\");
		std::wstring head = m_AniPath.substr(pos + 1, m_AniPath.length());
		m_AniPath = head + tail;	//마지막 경로 + 파일명



		AniPreview* player = ya::object::Instantiate<AniPreview>(eColliderLayer::Player);
		player->SetPos(Vector2(1550.f, 250.f));
		player->SetAniname(tail);
		player->SetAnipath(szFilePath);


	}






}

