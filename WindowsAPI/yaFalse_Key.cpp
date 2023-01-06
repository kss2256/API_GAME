#include "yaFalse_Key.h"
#include "yaInput.h"
#include "yaPlayer.h"
#include "yaCollider.h"
#include "yaCamera.h"
#include "yaTime.h"
#include "yaObject.h"
#include "yaFalse_Knight.h"
#include "yaAnimator.h"
#include "yaResources.h"
#include "yaImage.h"
#include "yaSound.h"

namespace ya
{
	float False_Key::s_fTimer = 0.f;

	False_Key::False_Key()
		: m_bCheak(false)
		, m_bEnd(false)
	{
		SetName(L"False_Key");
		SetPos({ 6214.0f, 3837.0f });
		//SetPos({ 500.0f, 500.0f });
		SetScale({ 1.0f, 1.0f });

		mAnimator = new Animator();

		Texture* Key = Resources::Load<Texture>(L"KeyImg", L"..\\Resources\\Image\\KeyImg.bmp");

		mAnimator->CreateAnimation(L"Key_Open", Key
			, Vector2(0.0f, 0.0f), Vector2(199.0f, 371.0f)
			, Vector2(0.0f, 0.0f), 9, 0.1f);


		m_Sound = Resources::Load<Sound>(L"stag_gate_open", L"..\\Resources\\Sound\\stag_gate_open.wav");

		m_Sound->Play(false);
		AddComponent(mAnimator);

		mAnimator->Play(L"Key_Open", false);

		Collider* collider = AddComponent<Collider>();
		collider->SetScale(Vector2(150.0f, 200.0f));
	}

	False_Key::~False_Key()
	{
	}


	void False_Key::Tick()
	{
		if (m_bCheak)
		{
			s_fTimer += DT;
			if (s_fTimer >= 1.0)
			{
				m_bEnd = true;
				m_bCheak = false;
				s_fTimer = 0.f;
			}


		}

		GameObject::Tick();
	}

	void False_Key::OnCollisionEnter(Collider* other)
	{


	}

	void False_Key::OnCollisionStay(Collider* other)
	{
		if (Player* playerObj = dynamic_cast<Player*>(other->GetOwner()))
		{
			if (KEY_DOWN(eKeyCode::UP))
			{				
				//���⼭ �÷��̾� ��ġ �̵� + False_Knight ��ȯ ���̵��� �ƿ� ȿ�� 
				playerObj->SetPlayerIn(true);
				m_bCheak = true;
				
			}

			if (m_bEnd)
			{

				State Player = playerObj->GetState();
				Player.bgm = PLAYER_BGM::False;
				playerObj->SetPos(Vector2(115.f, 4291.f));
				playerObj->SetState(Player);
				Camera::SetFalse(true);
				m_bEnd = false;
				False_Knight* false_knight = ya::object::Instantiate<False_Knight>(eColliderLayer::Monster);
				false_knight->SetPos(Vector2(1500.f, 4150.f));
				false_knight->SetTarget(playerObj);
				
			}

		}

	}

	void False_Key::OnCollisionExit(Collider* other)
	{
	}



}