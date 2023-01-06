#include "yaHornet_Attack.h"
#include "yaResources.h"
#include "yaImage.h"
#include "yaAnimator.h"
#include "yaCollider.h"
#include "yaTime.h"
#include "yaPlayer_Attack.h"
#include "yaCollisionManager.h"


namespace ya
{

	float Hornet_Attack::s_fTimer = 0.f;

	Hornet_Attack::Hornet_Attack()
		: m_pHornet(nullptr)
		, m_pMonsterSize(nullptr)
		, m_pImg(nullptr)
		, m_pAniAttack(nullptr)
		, m_pAniEffect(nullptr)
		, m_bTrapBreak(false)
		, m_vTargetPos(Vector2::Zero)
		, m_vFixPos(Vector2::Zero)
		, m_vMovePos(Vector2::Zero)
		, m_vAttackPos(Vector2::Zero)
		, m_eAttack(Attack_Type::None)
	{
		
	

		SetName(L"Hornet_Attack");
		SetPos(m_vTargetPos);
		SetScale({ 1.0f, 1.0f });


		this->SetDeathTime(4.f);

		m_pImg = Resources::Load<Texture>(L"Hornet_Effect", L"..\\Resources\\Image\\Hornet_Effect.bmp");

		m_pAniAttack = new Animator();
		m_pAniEffect = new Animator();

		AnimationInitialize();


		//m_pAniAttack->Play(L"Trap_Break2", true);
		//m_pAniEffect->Play(L"Right_Spear_Effect", true);
		

		AddComponent(m_pAniAttack);
		AddComponent(m_pAniEffect);

	}

	Hornet_Attack::~Hornet_Attack()
	{



	}


	void Hornet_Attack::Tick()
	{
		if (nullptr != m_pHornet)
			m_vTargetPos = m_pHornet->GetPos();
		

		switch (m_eAttack)
		{
		case Attack_Type::None:
			break;
		case Attack_Type::Left_Spear:
		{
			m_pMonsterSize = new Collider();
			AddComponent(m_pMonsterSize);
			m_pMonsterSize->SetScale(Vector2(260.0f, 50.0f));

			m_pAniAttack->Play(L"Left_Spear_Attack", false);
			m_eAttack = Attack_Type::None;
		}
			break;
		case Attack_Type::Right_Spear:
		{
			m_pMonsterSize = new Collider();
			AddComponent(m_pMonsterSize);
			m_pMonsterSize->SetScale(Vector2(260.0f, 50.0f));

			m_pAniAttack->Play(L"Right_Spear_Attack", false);
			m_eAttack = Attack_Type::None;
		}
		break;
		case Attack_Type::Left_Spear_Effect:
		{
			m_pAniEffect->Play(L"Left_Spear_Effect", false);
			m_eAttack = Attack_Type::None;
		}
		break;
		case Attack_Type::Right_Spear_Effect:
		{
			m_pAniEffect->Play(L"Right_Spear_Effect", false);
			m_eAttack = Attack_Type::None;
		}
		break;
		case Attack_Type::Trap:
		{
			
			switch (m_eTrap)
			{
			case Tarp_Type::Thread:
			{				
				m_pAniAttack->Play(L"Trap_Attack1", false);
				m_eTrap = Tarp_Type::Anticipate;
			}
				break;
			case Tarp_Type::Anticipate:
			{
				s_fTimer += DT;
				if (s_fTimer >= 0.5f)
				{
					m_pAniAttack->Play(L"Trap_Attack2", false);
					m_eTrap = Tarp_Type::Spike;
					s_fTimer = 0.f;
				}

			}
				break;
			case Tarp_Type::Spike:
			{
				s_fTimer += DT;
				if (s_fTimer >= 0.1f)
				{
					m_pMonsterSize = new Collider();
					AddComponent(m_pMonsterSize);
					m_pMonsterSize->SetScale(Vector2(70.0f, 70.0f));
					m_pAniAttack->Play(L"Trap_Attack3", false);
					s_fTimer = 0.f;
					m_eTrap = Tarp_Type::Spike_Waiting;
					
				}
			}
				break;
			case Tarp_Type::Spike_Waiting:
			{
				if (m_bTrapBreak)
				{
					Vector2 Pos = GetPos();
					//오른쪽으로 밀려야 함..
					if (m_vAttackPos.x > m_vMovePos.x)
					{
						if (m_vAttackPos.x + 100 >= Pos.x)
						{
							Pos.x += 600.f * DT;
							Pos.y -= 800.f * DT;

						}
						else
						{
							m_eTrap = Tarp_Type::Bard_Loose;
							m_pAniAttack->Play(L"Trap_Break2", false);
						}
					}

					//왼쪽으로 미밀려야 함..
					if (m_vAttackPos.x < m_vMovePos.x)
					{
						if (m_vAttackPos.x - 100 <= Pos.x)
						{
							m_vMovePos.x -= 600.f * DT;
							Pos.y -= 800.f * DT;
						}
						else
						{
							m_eTrap = Tarp_Type::Bard_Loose;
							m_pAniAttack->Play(L"Trap_Break2", false);
						}
					}
					SetPos(Pos);
				}
			}
			break;


			case Tarp_Type::Bard_Loose:
			{
				s_fTimer += DT;
				if (s_fTimer >= 0.3f)
				{
					CollisionManager::SetLayer(eColliderLayer::Monster_Projecttile, eColliderLayer::Player, true);
					CollisionManager::SetLayer(eColliderLayer::Monster_Projecttile, eColliderLayer::Player_Projecttile, true);
					s_fTimer = 0.f;
					m_eAttack = Attack_Type::None;
					this->Death();
				}
			}
				break;

			}	

		}
		break;
	}	

		GameObject::Tick();
	}




	void Hornet_Attack::OnCollisionEnter(Collider* other)
	{
		if (Player_Attack* playerObj = dynamic_cast<Player_Attack*>(other->GetOwner()))
		{
			if (m_eTrap == Tarp_Type::Spike_Waiting)
			{
				m_bTrapBreak = true;
				m_pAniAttack->Play(L"Trap_Break1", true);
				m_vMovePos = other->GetPos();
				m_vAttackPos = GetPos();
				CollisionManager::SetLayer(eColliderLayer::Monster_Projecttile, eColliderLayer::Player, false);
				CollisionManager::SetLayer(eColliderLayer::Monster_Projecttile, eColliderLayer::Player_Projecttile, false);
			}
			//--m_Hp;
			//m_tHitSaveAi = m_tMonster;
			//if (m_pPlayer != nullptr)
			//	m_vTargetPos = m_pPlayer->GetPos();
			//m_vMovePos = GetPos();
			//m_vAttackPos = GetPos();
			//m_tMonster.HIA = HornetAI::Dumi;
			//m_tMonster.HAction = HornetAction::None;
		}

	}
	void Hornet_Attack::OnCollisionStay(Collider* other)
	{



	}
	void Hornet_Attack::OnCollisionExit(Collider* other)
	{


	}



	void Hornet_Attack::AnimationInitialize()
	{
		//Spear 창 공격 
		m_pAniAttack->CreateAnimation(L"Left_Spear_Attack", m_pImg
			, Vector2(2980.0f, 740.0f), Vector2(281.0f, 22.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);
		m_pAniAttack->CreateAnimation(L"Right_Spear_Attack", m_pImg
			, Vector2(2980.0f, 762.0f), Vector2(281.0f, 22.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);


		//Spear_Effect 이펙트
		m_pAniEffect->CreateAnimation(L"Left_Spear_Effect", m_pImg
			, Vector2(0.0f, 2770.0f), Vector2(636.0f, 54.0f)
			, Vector2(620.0f, -10.0f), 7, 0.05f);
		m_pAniEffect->CreateAnimation(L"Right_Spear_Effect", m_pImg
			, Vector2(0.0f, 2824.0f), Vector2(636.0f, 54.0f)
			, Vector2(-620.0f, -10.0f), 7, 0.05f);


		//Trap 트랩 설치
		m_pAniAttack->CreateAnimation(L"Trap_Attack1", m_pImg
			, Vector2(2980.0f, 784.0f), Vector2(17.0f, 750.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		m_pAniAttack->CreateAnimation(L"Trap_Attack2", m_pImg
			, Vector2(3065.0f, 784.0f), Vector2(23.0f, 737.0f)
			, Vector2(0.0f, 0.0f), 1, 0.1f);
		m_pAniAttack->CreateAnimation(L"Trap_Attack3", m_pImg
			, Vector2(3088.0f, 784.0f), Vector2(138.0f, 750.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);

		//Trap Break
		m_pAniAttack->CreateAnimation(L"Trap_Break1", m_pImg
			, Vector2(3502.0f, 784.0f), Vector2(99.0f, 101.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		m_pAniAttack->CreateAnimation(L"Trap_Break2", m_pImg
			, Vector2(3997.0f, 784.0f), Vector2(99.0f, 102.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);

	}

}