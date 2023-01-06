#include "yaPlayer_Attack.h"
#include "yaPlayer.h"
#include "yaCollider.h"
#include "yaHornet.h"
#include "yaHusk.h"
#include "yaBaldur.h"
#include "yaTime.h"
#include "yaImage.h"
#include "yaResources.h"
#include "yaAnimator.h"

namespace ya
{
	float Player_Attack::s_fTimer = 0.f;

	Player_Attack::Player_Attack()
		: m_eCompass(COMPASS::None)
		, m_pPlayer(nullptr)
		, m_pAttack(nullptr)
		, m_pTexture(nullptr)
		, m_bMagicHit(false)
		, m_eSkill()
	{
		SetName(L"Player_Attack");			
		SetScale({ 1.0f, 1.0f });

		m_pAttack = new Animator();
		AddComponent(m_pAttack);

		m_pCollider = new Collider();
		AddComponent(m_pCollider);


		m_pCollider->SetOffset(Vector2(0.0f, 0.0f));
		m_pCollider->SetScale(Vector2(100.0f, 100.0f));

		if (nullptr == m_pTexture)
		{
			m_pTexture = Resources::Load<Texture>(L"Player_Magic", L"..\\Resources\\Image\\Player_Magic.bmp");
		}


		m_pAttack->CreateAnimation(L"Left_Magic", m_pTexture
			, Vector2(0.0f, 0.0f), Vector2(263.0f, 113.0f)
			, Vector2(-30.0f, 0.0f), 6, 0.1f);
		m_pAttack->CreateAnimation(L"Right_Magic", m_pTexture
			, Vector2(0.0f, 113.0f), Vector2(263.0f, 113.0f)
			, Vector2(30.0f, 0.0f), 6, 0.1f);

		m_pAttack->CreateAnimation(L"Left_Magic_Hit", m_pTexture
			, Vector2(0.0f, 226.0f), Vector2(200.0f, 186.0f)
			, Vector2(0.0f, 0.0f), 8, 0.05f);
		m_pAttack->CreateAnimation(L"Right_Magic_Hit", m_pTexture
			, Vector2(0.0f, 412.0f), Vector2(200.0f, 186.0f)
			, Vector2(0.0f, 0.0f), 8, 0.05f);


	}

	Player_Attack::~Player_Attack()
	{

	}





	void Player_Attack::Tick()
	{
		if (nullptr != m_pPlayer)
			SetPos(m_pPlayer->GetPos());

		switch (m_eSkill)
		{
		case ya::Player_Skill::Attack:
		{
			switch (m_eCompass)
			{
			case ya::COMPASS::Up:
			{
				m_pCollider->SetOffset(Vector2(10.0f, -120.0f));
				m_pCollider->SetScale(Vector2(140.0f, 120.0f));
				s_fTimer += DT;
				if (s_fTimer >= 0.16f)
				{
					s_fTimer = 0.f;
					this->Death();
				}

			}
			break;
			case ya::COMPASS::Down:
			{
				m_pCollider->SetOffset(Vector2(-10.0f, 120.0f));
				m_pCollider->SetScale(Vector2(140.0f, 120.0f));
				s_fTimer += DT;
				if (s_fTimer >= 0.16f)
				{
					s_fTimer = 0.f;
					this->Death();
				}
			}
			break;
			case ya::COMPASS::Left:
			{
				m_pCollider->SetOffset(Vector2(-100.0f, 0.0f));
				m_pCollider->SetScale(Vector2(140.0f, 140.0f));
				s_fTimer += DT;
				if (s_fTimer >= 0.2f)
				{
					s_fTimer = 0.f;
					this->Death();
				}
			}
			break;
			case ya::COMPASS::Right:
			{
				m_pCollider->SetOffset(Vector2(100.0f, 0.0f));
				m_pCollider->SetScale(Vector2(140.0f, 140.0f));
				s_fTimer += DT;
				if (s_fTimer >= 0.2f)
				{
					s_fTimer = 0.f;
					this->Death();
				}
			}
			break;
			}
		}
			break;
		case ya::Player_Skill::Magic:
		{


			if (m_eCompass == COMPASS::Left)
			{
				m_pAttack->Play(L"Left_Magic", true);
				m_pCollider->SetOffset(Vector2(-90.0f, 0.0f));
				m_pCollider->SetScale(Vector2(140.0f, 113.0f));
			}
			if (m_eCompass == COMPASS::Right)
			{
				m_pAttack->Play(L"Right_Magic", true);
				m_pCollider->SetOffset(Vector2(90.0f, 0.0f));
				m_pCollider->SetScale(Vector2(140.0f, 113.0f));
			}

			m_vMovePos = GetPos();
			m_eSkill = Player_Skill::Magic_Walk;
		}
			break;	
		case ya::Player_Skill::Magic_Walk:
		{
			s_fTimer += DT;
			if (s_fTimer >= 1.5)
			{
				this->Death();
				s_fTimer = 0.f;
			}

			if (m_eCompass == COMPASS::Left)
			{				
				m_vMovePos.x -= 1200.f * DT;
			}
			if (m_eCompass == COMPASS::Right)
			{
				m_vMovePos.x += 1200.f * DT;
			}
			SetPos(m_vMovePos);

			if (m_bMagicHit)
			{
				m_vMovePos = GetPos();
				int a = 0;
				s_fTimer = 0.f;
				m_bMagicHit = false;
				m_eSkill = Player_Skill::Magic_Hit;
				
			}
			
			
		}
		break;
		case ya::Player_Skill::Magic_Hit:
		{
			if (m_eCompass == COMPASS::Left)
				m_pAttack->Play(L"Left_Magic_Hit", false);
			if (m_eCompass == COMPASS::Right)
				m_pAttack->Play(L"Right_Magic_Hit", false);

			m_eSkill = Player_Skill::Magic_Death;

		}
		break;
		case ya::Player_Skill::Magic_Death:
		{
			SetPos(m_vMovePos);
			this->SetDeathTime(float(0.35));

		}
		break;
		}


		//2√  , 1.6√ 
	



		GameObject::Tick();
	}

	void Player_Attack::OnCollisionEnter(Collider* other)
	{



	}

	void Player_Attack::OnCollisionStay(Collider* other)
	{



	}

	void Player_Attack::OnCollisionExit(Collider* other)
	{


	}

}
