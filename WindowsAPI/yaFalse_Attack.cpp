#include "yaFalse_Attack.h"
#include "yaImage.h"
#include "yaFalse_Knight.h"
#include "yaCollider.h"
#include "yaTime.h"
#include "yaAnimator.h"
#include "yaCollisionManager.h"
#include "yaResources.h"

namespace ya
{
	float False_Attack::s_fTimer = 0.f;

	False_Attack::False_Attack()
		: m_pImg(nullptr)
		, m_pAniAttack(nullptr)
		, m_pMonsterSize(nullptr)
		, m_pFalse(nullptr)
		, m_vTargetPos(Vector2::Zero)
		, m_vFixPos(Vector2::Zero)
		, m_vMovePos(Vector2::Zero)
		, m_vAttackPos(Vector2::Zero)
		, m_bShock(false)
		, m_bEarthquak(false)
		, m_eSkill()
	{

		SetName(L"False_Attack");
		SetPos(m_vTargetPos);
		SetScale({ 1.0f, 1.0f });

		//this->SetDeathTime(5.f);

		m_pImg = Resources::Load<Texture>(L"Earthquake_Effect", L"..\\Resources\\Image\\Earthquake_Effect.bmp");

		m_pAniAttack = new Animator();
	
		m_pMonsterSize = new Collider();
		AddComponent(m_pMonsterSize);

		AnimationInitialize();


		//m_pAniAttack->Play(L"Trap_Break2", true);
		//m_pAniEffect->Play(L"Right_Spear_Effect", true);


		AddComponent(m_pAniAttack);
		
		

	}

	False_Attack::~False_Attack()
	{
	}


	void False_Attack::Tick()
	{
		if (nullptr != m_pFalse)
			m_vTargetPos = m_pFalse->GetPos();

		

		if (m_bShock)
		{
			s_fTimer += DT;
			if (n_sDirection == Attack_Direction::Left)
			{
				if (s_fTimer >= 0.6)
				{
					m_pMonsterSize->SetScale(Vector2(510.0f, 370.0f));
					m_pMonsterSize->SetOffset(Vector2(0.0f, 0.0f));
					m_bShock = false;
					s_fTimer = 0.f;
				}
				else if (s_fTimer >= 0.4)
				{
					m_pMonsterSize->SetScale(Vector2(350.0f, 275.0f));
					m_pMonsterSize->SetOffset(Vector2(-100.0f, 50.0f));
				}
				else if (s_fTimer >= 0.2)
				{
					m_pMonsterSize->SetScale(Vector2(160.0f, 255.0f));
					m_pMonsterSize->SetOffset(Vector2(-170.0f, 60.0f));
				}
			}
			else
			{
				if (s_fTimer >= 0.6)
				{
					m_pMonsterSize->SetScale(Vector2(510.0f, 370.0f));
					m_pMonsterSize->SetOffset(Vector2(0.0f, 0.0f));
					m_bShock = false;
					s_fTimer = 0.f;
				}
				else if (s_fTimer >= 0.4)
				{
					m_pMonsterSize->SetScale(Vector2(350.0f, 275.0f));
					m_pMonsterSize->SetOffset(Vector2(100.0f, 50.0f));
				}
				else if (s_fTimer >= 0.2)
				{
					m_pMonsterSize->SetScale(Vector2(160.0f, 255.0f));
					m_pMonsterSize->SetOffset(Vector2(170.0f, 60.0f));
				}
			}
		}






		switch (m_eSkill)
		{
		case False_Skill::Earthquak:
		{
			this->SetDeathTime(2.5f);
			m_pAniAttack->Play(L"Earthquake", true);
			m_vMovePos = GetPos();
			m_pMonsterSize->SetScale(Vector2(159.0f, 146.0f));
			m_eSkill = False_Skill::Earthquak_Walk;

		}
		break;
		case False_Skill::Earthquak_Walk:
		{
			m_vMovePos.y += 600.f * DT;
			SetPos(m_vMovePos);


		}
		break;
		case False_Skill::Shock_Wave:
		{
			this->SetDeathTime(2.5f);
			m_vAttackPos = GetPos();
			if (n_sDirection == Attack_Direction::Left)
			{
				m_pAniAttack->Play(L"Shock_Left_Effect", false);
				m_eSkill = False_Skill::Shock_Walk;
				m_pMonsterSize->SetScale(Vector2(160.0f, 220.0f));
				m_pMonsterSize->SetOffset(Vector2(-170.0f, 75.0f));
			}
			if (n_sDirection == Attack_Direction::Right)
			{
				m_pAniAttack->Play(L"Shock_Right_Effect", false);
				m_eSkill = False_Skill::Shock_Walk;
				m_pMonsterSize->SetScale(Vector2(160.0f, 220.0f));
				m_pMonsterSize->SetOffset(Vector2(170.0f, 75.0f));
			}

			m_bShock = true;
		}
		break;
		case False_Skill::Shock_Walk:
		{
			m_vMovePos = GetPos();
			if (n_sDirection == Attack_Direction::Left)
			{
				m_vMovePos.x -= 1200.f * DT;
			}
			if (n_sDirection == Attack_Direction::Right)
			{
				m_vMovePos.x += 1200.f * DT;
			}
			SetPos(m_vMovePos);
		}
		break;
		case False_Skill::Attack:
		{
			m_vAttackPos = GetPos();
			if (n_sDirection == Attack_Direction::Left)
			{
				m_pMonsterSize->SetScale(Vector2(300.0f, 300.0f));
				m_pMonsterSize->SetOffset(Vector2(-300.0f, 50.0f));
				m_eSkill = False_Skill::Attack_Walk;
			}
			if (n_sDirection == Attack_Direction::Right)
			{
				m_pMonsterSize->SetScale(Vector2(300.0f, 300.0f));
				m_pMonsterSize->SetOffset(Vector2(300.0f, 50.0f));
				m_eSkill = False_Skill::Attack_Walk;
			}
			
		}
		break;
		case False_Skill::Attack_Walk:
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.4)
			{
				this->Death();
				s_fTimer = 0.f;
			}

		}
		break;
		case False_Skill::Stun:
		{
			m_vAttackPos = GetPos();
			if (n_sDirection == Attack_Direction::Left)
			{
				m_pAniAttack->Play(L"False_Left_Body", false);
			}
			if (n_sDirection == Attack_Direction::Right)
			{
				m_pAniAttack->Play(L"False_Right_Body", false);
			}

		}
		break;
		}

	

		GameObject::Tick();
	}

	void False_Attack::OnCollisionEnter(Collider* other)
	{


	}

	void False_Attack::OnCollisionStay(Collider* other)
	{


	}

	void False_Attack::OnCollisionExit(Collider* other)
	{


	}

	void False_Attack::AnimationInitialize()
	{

		m_pAniAttack->CreateAnimation(L"Shock_Left_Effect", m_pImg
			, Vector2(0.0f, 0.0f), Vector2(510.0f, 370.0f)
			, Vector2(0.0f, 0.0f), 4, 0.2f);
		m_pAniAttack->CreateAnimation(L"Shock_Right_Effect", m_pImg
			, Vector2(0.0f, 370.0f), Vector2(510.0f, 370.0f)
			, Vector2(0.0f, 0.0f), 4, 0.2f);


		m_pAniAttack->CreateAnimation(L"Earthquake", m_pImg
			, Vector2(0.0f, 740.0f), Vector2(159.0f, 146.0f)
			, Vector2(0.0f, 0.0f), 7, 0.2f);


		m_pAniAttack->CreateAnimation(L"False_Left_Body", m_pImg
			, Vector2(347.0f, 886.0f), Vector2(347.0f, 259.0f)
			, Vector2(0.0f, 90.0f), 1, 0.1f);
		m_pAniAttack->CreateAnimation(L"False_Right_Body", m_pImg
			, Vector2(0.0f, 886.0f), Vector2(347.0f, 259.0f)
			, Vector2(0.0f, 90.0f), 1, 0.1f);


	}


}

