#include "yaHornet_Key.h"
#include "yaInput.h"
#include "yaPlayer.h"
#include "yaCollider.h"
#include "yaCamera.h"
#include "yaTime.h"
#include "yaObject.h"
#include "yaHornet.h"
#include "yaImage.h"
#include "yaAnimator.h"
#include "yaResources.h"
#include "yaSound.h"

namespace ya
{
	float Hornet_Key::s_fTimer = 0.f;

	Hornet_Key::Hornet_Key()
		: m_bCheak(false)
		, m_bEnd(false)
	{
		SetName(L"Hornet_Key");
		SetPos({ 10150.0f, 3600.0f });
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

	Hornet_Key::~Hornet_Key()
	{
	}

	void Hornet_Key::Tick()
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

	void Hornet_Key::OnCollisionEnter(Collider* other)
	{
	}

	void Hornet_Key::OnCollisionStay(Collider* other)
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
				Player.bgm = PLAYER_BGM::Hornet;
				playerObj->SetPos(Vector2(2942.f, 4316.f));
				playerObj->SetState(Player);
				Camera::SetHornet(true);
				m_bEnd = false;
				Hornet* hornet = ya::object::Instantiate<Hornet>(eColliderLayer::Monster);
				hornet->SetPos(Vector2(3800.f, 4260.f));
				hornet->SetTarget(playerObj);

			}

		}

	}

	void Hornet_Key::OnCollisionExit(Collider* other)
	{
	}



}