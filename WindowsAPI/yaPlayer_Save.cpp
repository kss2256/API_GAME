#include "yaPlayer_Save.h"
#include "yaCollider.h"
#include "yaResources.h"
#include "yaImage.h"
#include "yaCamera.h"
#include "yaPlayer.h"
#include "yaInput.h"
#include "yaSound.h"


namespace ya
{
	Player_Save::Player_Save()
		: m_Img(nullptr)
		, m_pSound(nullptr)
	{
		SetName(L"Player_Save");
		SetPos({ 8613.0f, 3740.0f });
		//SetPos({ 500.0f, 500.0f });
		SetScale({ 1.0f, 1.0f });

		if(m_Img == nullptr)
		m_Img = ya::Resources::Load<Texture>(L"Object2", L"..\\Resources\\Image\\Object2.bmp");

		m_pSound = Resources::Load<Sound>(L"Player_Heal_S1", L"..\\Resources\\Sound\\Player_Heal_S1.wav");

		Collider* collider = AddComponent<Collider>();
		collider->SetScale(Vector2(150.0f, 20.0f));

		
	}

	Player_Save::~Player_Save()
	{
	}

	void Player_Save::Tick()
	{

		GameObject::Tick();
	}

	void Player_Save::Render(HDC _hdc)
	{
		Vector2 Pos = Camera::CalculatePos(GetPos());

		BLENDFUNCTION tFunc = {};
		tFunc.BlendOp = AC_SRC_OVER;
		tFunc.BlendFlags = 0;
		tFunc.AlphaFormat = AC_SRC_ALPHA;
		tFunc.SourceConstantAlpha = 255;


		if (nullptr != m_Img)
		{
			AlphaBlend(_hdc
				, (int)Pos.x - m_Img->GetWidth() / 2
				, (int)Pos.y - m_Img->GetHeight() / 2
				, m_Img->GetWidth(), m_Img->GetHeight()
				, m_Img->GetDC(), 0, 0
				, m_Img->GetWidth(), m_Img->GetHeight(), tFunc);
		}

		GameObject::Render(_hdc);
	}

	void Player_Save::OnCollisionEnter(Collider* other)
	{


	}

	void Player_Save::OnCollisionStay(Collider* other)
	{
		if (Player* playerObj = dynamic_cast<Player*>(other->GetOwner()))
		{
			
			if (KEY_DOWN(eKeyCode::UP))
			{
				playerObj->SetPlayerHeal(true);
				m_pSound->Play(false);
			}

			

	

			
		}

	}

	void Player_Save::OnCollisionExit(Collider* other)
	{


	}



}