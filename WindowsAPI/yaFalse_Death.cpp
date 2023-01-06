#include "yaFalse_Death.h"
#include "yaCollider.h"
#include "yaTime.h"
#include "yaPlayer.h"
#include "yaInput.h"
#include "yaCamera.h"

namespace ya
{

	float False_Potar::s_fTimer = 0.f;

	False_Potar::False_Potar()
		: m_bCheak(false)
		, m_bEnd(false)
	{	 

		SetName(L"False_Portal");
		//SetPos({ 6214.0f, 3837.0f });
		//SetPos({ 500.0f, 500.0f });
		SetScale({ 1.0f, 1.0f });


		Collider* collider = AddComponent<Collider>();
		collider->SetScale(Vector2(150.0f, 200.0f));

	}

	False_Potar::~False_Potar()
	{
	}



	void False_Potar::Tick()
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

	void False_Potar::OnCollisionEnter(Collider* other)
	{
	}

	void False_Potar::OnCollisionStay(Collider* other)
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
				Player.bgm = PLAYER_BGM::Back;				
				playerObj->SetPos(playerObj->GetFixPos());
				playerObj->SetState(Player);
				Camera::SetFalse(false);				

				m_bEnd = false;


			}

		}

	}

	void False_Potar::OnCollisionExit(Collider* other)
	{
	}


}