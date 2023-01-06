
#include "yaButton.h"
#include "yaImage.h"
#include "yaInput.h"
#include "yaCamera.h"
#include "yaResources.h"
#include "yaLogoScene.h"
#include "yaTime.h"
#include "yaSoundManager.h"
#include "yaSound.h"




namespace ya
{

	bool Button::m_bSound = false;

	Button::Button(eUIType type)
		: UiBase(type)
		, m_pImg(nullptr)
		, m_eMode(eModeType::None)
		, m_pLogo(nullptr)
		, m_bOverlap(false)

	{
		mOnClick = std::bind(&Button::Click, this);
		mOnMouseDown = std::bind(&Button::MouseDown, this);





	}
	Button::~Button()
	{
		if (nullptr != m_pLogo)
			delete m_pLogo;

		if (m_Sound != nullptr)
			delete m_Sound;
	}

	void Button::OnInit()
	{


	}

	void Button::OnActive()
	{

	}

	void Button::OnInActive()
	{

	}

	void Button::OnTick()
	{

		if (m_Sound == nullptr)
			m_Sound = Resources::Load<Sound>(L"Logo", L"..\\Resources\\Sound\\Logo.wav");
		if(m_bSound == false)
		{
			if (m_Sound != nullptr)
			{
				m_Sound->Play(true);
				m_bSound = true;
			}


		}



		Vector2 mousePos = Input::GetMousePos();
		Vector2 size = GetSize();
		
		bool bLbtnPressed = KEY_DOWN(eKeyCode::LBTN);
		bool bLbtnReleased = KEY_UP(eKeyCode::LBTN);



		if (mScreenPos.x <= mousePos.x && mousePos.x < mScreenPos.x + size.x
			&& mScreenPos.y <= mousePos.y && mousePos.y < mScreenPos.y + size.y)
		{
			mbMouseOn = true;
			MouseOn();
		}
		else
		{
			mbMouseOn = false;
		}

		if (bLbtnPressed && mbMouseOn)
		{
			mOnMouseDown();

		}
		if (bLbtnReleased && mbMouseOn)
		{
			mOnClick();
		}
	}

	void Button::OnRender(HDC hdc)
	{
		BLENDFUNCTION func = {};
		func.BlendOp = AC_SRC_OVER;
		func.BlendFlags = 0;
		func.AlphaFormat = AC_SRC_ALPHA;
		func.SourceConstantAlpha = 255;


		AlphaBlend(hdc, (int)mScreenPos.x, (int)mScreenPos.y
			, mImage->GetWidth(), mImage->GetHeight()
			, mImage->GetDC(), 0, 0, mImage->GetWidth(), mImage->GetHeight(), func);


		static float fRatio = 1.f;
		static float Dir = -1.f;
		m_iOverlap = (int)((255) * (fRatio));

		if (m_bOverlap)
		{
			m_time += DT;			//깜빡임 지속 시간 타이머
			fRatio += DT * Dir * 20.f;

			if (0.f > fRatio)
			{
				fRatio = 0.f;
				Dir = 1.f;
			}
			else if (fRatio > 1.f)
			{
				fRatio = 1.f;
				Dir = -1;
			}
			if (m_time > 0.8f)		//0.8초 후 다시 원래 상태로 돌림
			{
				m_bOverlap = false;
				m_time = 0;
				fRatio = 1.f;
				Dir = -1.f;
			}
		}	

		if (m_pImg)
		{

			BLENDFUNCTION func = {};
			func.BlendOp = AC_SRC_OVER;
			func.BlendFlags = 0;
			func.AlphaFormat = AC_SRC_ALPHA;
			func.SourceConstantAlpha = m_iOverlap;


			AlphaBlend(hdc
				, (int)m_VecPos.x
				, (int)m_VecPos.y
				, m_pImg->GetWidth(), m_pImg->GetHeight()
				, m_pImg->GetDC(), 0, 0
				, m_pImg->GetWidth(), m_pImg->GetHeight(), func);
		}
	}

	void Button::OnClear()
	{




	}

	void Button::Click()
	{

		//if (m_pInst && m_pDelegateFunc)
		//{
		//	(m_pInst->*m_pDelegateFunc)();
		//}

		if (nullptr == m_pLogo)
			m_pLogo = new LogoScene;

		m_bOverlap = true;

	
	

		switch (m_eMode)
		{

		case eModeType::Play:
			m_Sound->Stop(true);
			
			m_pLogo->Start();
			break;

		case eModeType::Option:

			break;

		case eModeType::Ground:

			m_Sound->Stop(true);
			
			m_pLogo->Ground();
			break;

		case eModeType::Animation:
			m_Sound->Stop(true);
			
			m_pLogo->Animation();
			break;

		case eModeType::Exit:
			m_pLogo->Exit();
			break;


		}
	}
	void Button::MouseDown()
	{


	}
	void Button::MouseOn()
	{
		Vector2 Pos = GetPos();
		Vector2 mousePos = MOUSE_POS;

		m_pImg = Resources::Load<Texture>(L"LogoPick", L"..\\Resources\\Image\\LogoPick.bmp");

		if (mousePos.y < Pos.y + 59 && mousePos.y > Pos.y)
		{
			m_VecPos = Vector2(Pos.x, Pos.y + 2.5f);
			m_eMode = eModeType::Play;
		}
		if (mousePos.y < Pos.y + 118 && mousePos.y > Pos.y + 59)
		{
			m_VecPos = Vector2(Pos.x, Pos.y + 68.5f);
			m_eMode = eModeType::Option;
		}
		if (mousePos.y < Pos.y + 177 && mousePos.y > Pos.y + 118)
		{
			m_VecPos = Vector2(Pos.x, Pos.y + 132.5f);
			m_eMode = eModeType::Ground;
		}
		if (mousePos.y < Pos.y + 236 && mousePos.y > Pos.y + 177)
		{
			m_VecPos = Vector2(Pos.x, Pos.y + 198.5f);
			m_eMode = eModeType::Animation;
		}
		if (mousePos.y < Pos.y + 295 && mousePos.y > Pos.y + 236)
		{
			m_VecPos = Vector2(Pos.x, Pos.y + 265.5f);
			m_eMode = eModeType::Exit;
		}






	}
}
