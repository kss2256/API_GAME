#include "yaFalse_Knight.h"
#include "yaImage.h"
#include "yaResources.h"
#include "yaAnimator.h"
#include "yaCollider.h"
#include "yaTime.h"
#include "yaCamera.h"
#include "yaApplication.h"
#include "yaFalse_Attack.h"
#include "yaObject.h"
#include "yaRigidbody.h"
#include "yaPlayer_Attack.h"
#include "yaPlayer.h"
#include "yaCollisionManager.h"
#include "yaSound.h"
#include "yaFalse_Death.h"
#include <Time.h>

namespace ya
{
	float False_Knight::s_fTimer = 0.f;
	int False_Knight::s_iRandom = 0;
	int False_Knight::s_Attack_Repeat = 0;
	bool False_Knight::s_bStart = false;
	
	FALSE_AI FALSE_AI::Zero = {};

	False_Knight::False_Knight()
		: m_tMonster()
		, m_pPlayer(nullptr)
		, m_pFalseImg(nullptr)
		, m_pCollider(nullptr)
		, m_sShock(nullptr)
		, m_sSwing(nullptr)
		, m_sStrike(nullptr)
		, m_sJump(nullptr)
		, m_sLand(nullptr)
		, m_sJump_Attack(nullptr)
		, m_sShoulder(nullptr)
		, m_sEarthquake(nullptr)
		, m_sEarthquake1(nullptr)
		, m_sHit(nullptr)
		, m_sStun(nullptr)
		, m_sHead(nullptr)
		, m_Hp(30)
		, m_StunHp(6)
		, m_bTar(false)
		, m_StunCheak(false)
		, m_False_Death(false)
	{

		SetName(L"False_Knight");
		SetPos({ 900.0f, 500.0f });
		SetScale({ 1.0f, 1.0f });

		m_pFalseAttack = new Animator();
		m_pFalseEffect = new Animator();
		m_pCollider = new Collider();

		m_pCollider->SetScale(Vector2(400.f, 400.f));
		m_pCollider->SetOffset(Vector2(20.f, 0.f));
		AddComponent(m_pFalseAttack);
		AddComponent(m_pFalseEffect);
		AddComponent(m_pCollider);

		if(m_pFalseImg == nullptr)
		m_pFalseImg = Resources::Load<Texture>(L"False_Knight", L"..\\Resources\\Image\\False_Knight.bmp");
		
		

		AnimationInitialize();
		SoundInitialize();
		

		srand((UINT)time(nullptr));
		int dummy = rand();
		

		m_tMonster.Skill = FALSE_SKILL::End;
		m_tMonster.Progress = PROGRESS::start;
		m_tMonster.Direction = DIRECTION::Left;
		//m_tMonster.Direction = DIRECTION::Right;
		
		m_pFalseAttack->Play(L"False_Left_Standing", true);
		//m_pFalseEffect->Play(L"tes1t", true);

		AddComponent<Rigidbody>();
		Rigidbody* rigidbody = GetComponent<Rigidbody>();
		rigidbody->SetGround(true);

	}

	False_Knight::~False_Knight()
	{
	}

	void False_Knight::Tick()
	{
		
		
		if (s_bStart == false)
		{
			s_fTimer += DT;
			if (s_fTimer >= 4.5 && s_fTimer <= 4.51)
			{
				
			}

			if (s_fTimer >= 7)
			{
				s_bStart = true;
				s_fTimer = 0.f;
			}

		}


		if (s_bStart)
		{

			if (nullptr != m_pPlayer)
			{
				m_vTargetPos = m_pPlayer->GetPos();
				Vector2 Area = GetPos();
				if (abs(Area.x - m_vTargetPos.x) < 2200 && abs(Area.y - m_vTargetPos.y) < 2200)
					m_bTar = true;
				else
					m_bTar = false;
			}
		}
		

		if (m_bTar)
		{
			switch (m_tMonster.Skill)
			{
			case ya::FALSE_SKILL::None:
			{
				if (s_iRandom)
				{
					if (s_iRandom == 2)	//Hammer_Attack
					{
						//if (TargetPlayer())	
						//	False_Walk(500);
						//else
						//	False_Turn();
						False_Walk(200);
					}
					if (s_iRandom == 3) //Hammer_Earthquake
					{
						//if (TargetPlayer())
						//	False_BackJump(750);
						//else
						//	False_Turn();		
						False_BackJump(450);
					}
					if (s_iRandom == 4)//Hammer_Jump
					{
						//if (TargetPlayer())
						//	False_Walk(700);
						//else
						//	False_Turn();					
						False_Walk(400);
					}
					if (s_iRandom == 5)//Hammer_Shock_Wave
					{
						//if (TargetPlayer())
						//	False_BackJump(550);
						//else
						//	False_Turn();		
						False_BackJump(250);
					}
					if (s_iRandom == 6)//Shoulder_Attack
					{
						//if (TargetPlayer())
						//	False_BackJump(500);
						//else
						//	False_Turn();
						False_BackJump(200);
					}
				}
			}
			break;
			case ya::FALSE_SKILL::Stun:
			{
				m_StunCheak = true;
				Stun();
			}
				break;
			case ya::FALSE_SKILL::Death:
			{
				m_False_Death = true;
				False_Death();
			}
			break;
			case ya::FALSE_SKILL::Hammer_Attack:
			{
				Hammer_Attack();
			}
			break;
			case ya::FALSE_SKILL::Hammer_Earthquake:
			{
				int ran = rand() % 7 + 4;
				Hammer_Earthquake(ran);
			}
			break;
			case ya::FALSE_SKILL::Hammer_Jump:
			{
				Jump_Attack();
			}
			break;
			case ya::FALSE_SKILL::Hammer_Shock_Wave:
			{
				Hammer_Shock_Wave();
			}
			break;			
			case ya::FALSE_SKILL::Shoulder_Attack:
			{
				Shoulder_Attack();
			}
			break;
			case ya::FALSE_SKILL::End:
			{		
					//(최대 - 최소 + 1) + 최소
					s_iRandom = rand() % (6 - 2 + 1) + 2;
					m_tMonster.Skill = FALSE_SKILL::None;
			}
			break;

			}




		}

		
		GameObject::Tick();
	}
	void False_Knight::Render(HDC hdc)
	{



		GameObject::Render(hdc);
	}




	void False_Knight::OnCollisionEnter(Collider* other)
	{
		if (Player_Attack* playerObj = dynamic_cast<Player_Attack*>(other->GetOwner()))
		{			
			

			if (m_Hp <= 15 && m_StunCheak == false)
			{
				m_tMonster.Skill = FALSE_SKILL::Stun;
				m_tMonster.Progress = PROGRESS::start;
				m_tMonster.Hit = true;
			}
			else if (m_Hp <= 0 && m_False_Death == false)
			{
				CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, false);
				CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, false);
				m_tMonster.Skill = FALSE_SKILL::Death;
				m_tMonster.Progress = PROGRESS::start;
				m_tMonster.Hit = true;
				return;
			}
			else if (m_StunCheak && m_StunHp > 0)
			{
				--m_StunHp;
				m_tMonster.Hit = true;
				m_sHead->Play(false);
				return;
			}

			if (playerObj->GetSkill() == Player_Skill::Magic || playerObj->GetSkill() == Player_Skill::Magic_Walk)
			{
				playerObj->SetHit(true);
				if (m_Hp > 2)
				{
					m_Hp -= 3;
					m_sHit->Play(false);
				}
				else if (m_Hp < 3)
				{
					m_Hp = 0;
					m_sHit->Play(false);
				}
			}
			else
			{
				m_sHit->Play(false);
				--m_Hp;
			}


				m_tMonster.Hit = true;

				if (nullptr != m_pPlayer)
				{
					Player* play = (Player*)m_pPlayer;
					if (play->GetMp() >= 6)
						return;
					int mp = play->GetMp();
					++mp;
					play->SetMp(mp);
				}
		}

	}
	void False_Knight::OnCollisionStay(Collider* other)
	{
	}
	void False_Knight::OnCollisionExit(Collider* other)
	{
		if (Player_Attack* playerObj = dynamic_cast<Player_Attack*>(other->GetOwner()))
		{
			m_tMonster.Hit = false;
		}
	}









	void False_Knight::AnimationInitialize()
	{

		//기본 서있는 모션
		m_pFalseAttack->CreateAnimation(L"False_Left_Standing", m_pFalseImg
			, Vector2(0.0f, 0.0f), Vector2(621.0f, 396.0f)
			, Vector2(75.0f, 0.0f), 5, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Standing", m_pFalseImg
			, Vector2(0.0f, 396.0f), Vector2(621.0f, 396.0f)
			, Vector2(-75.0f, 0.0f), 5, 0.1f);

		//Turn
		m_pFalseAttack->CreateAnimation(L"False_Right_Turn", m_pFalseImg
			, Vector2(0.0f, 792.0f), Vector2(548.0f, 392.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Left_Turn", m_pFalseImg
			, Vector2(1096.0f, 792.0f), Vector2(548.0f, 392.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);

		//뛰는 모션 (2파트) + 걷는거 조합 = Run1 + Walk 1
		m_pFalseAttack->CreateAnimation(L"False_Left_Run1", m_pFalseImg
			, Vector2(0.0f, 1184.0f), Vector2(574.0f, 380.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Run1", m_pFalseImg
			, Vector2(1148.0f, 1184.0f), Vector2(574.0f, 380.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Left_Run2", m_pFalseImg
			, Vector2(0.0f, 1564.0f), Vector2(623.0f, 492.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Run2", m_pFalseImg
			, Vector2(0.0f, 2056.0f), Vector2(623.0f, 492.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Left_Walk", m_pFalseImg
			, Vector2(623.0f, 1564.0f), Vector2(623.0f, 492.0f)
			, Vector2(0.0f, -50.0f), 4, 0.15f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Walk", m_pFalseImg
			, Vector2(623.0f, 2056.0f), Vector2(623.0f, 492.0f)
			, Vector2(0.0f, -50.0f), 4, 0.15f);



		
		//점프 하는 모션 Jump (3파트)
		m_pFalseAttack->CreateAnimation(L"False_Left_Jump1", m_pFalseImg
			, Vector2(0.0f, 2548.0f), Vector2(610.0f, 383.0f)
			, Vector2(0.0f, 0.0f), 3, 0.25f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Jump1", m_pFalseImg
			, Vector2(0.0f, 2931.0f), Vector2(610.0f, 383.0f)
			, Vector2(0.0f, 0.0f), 3, 0.25f);
		m_pFalseAttack->CreateAnimation(L"False_Left_Jump2", m_pFalseImg
			, Vector2(0.0f, 3314.0f), Vector2(604.0f, 340.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Jump2", m_pFalseImg
			, Vector2(0.0f, 3654.0f), Vector2(604.0f, 340.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Left_Jump3", m_pFalseImg
			, Vector2(0.0f, 3994.0f), Vector2(610.0f, 383.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Jump3", m_pFalseImg
			, Vector2(0.0f, 4377.0f), Vector2(610.0f, 383.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);

		//기본 공격 하는 모션 (3피스) + 양쪽 공격에 필요한 중간 회수 모션? (1피스)
		//잘 조합해서 양쪽 공격하는거 만들면 될듯!  기모아서 공격은 어택 1번을 
		//5개로 2번그림 부터 시작 하면 됨
		m_pFalseAttack->CreateAnimation(L"False_Left_Attack1", m_pFalseImg
			, Vector2(0.0f, 4760.0f), Vector2(582.0f, 402.0f)
			, Vector2(150.0f, 0.0f), 6, 0.15f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Attack1", m_pFalseImg
			, Vector2(0.0f, 5162.0f), Vector2(582.0f, 402.0f)
			, Vector2(-150.0f, 0.0f), 6, 0.15f);
		m_pFalseAttack->CreateAnimation(L"False_Left_Attack2", m_pFalseImg
			, Vector2(0.0f, 5564.0f), Vector2(705.0f, 595.0f)
			, Vector2(-100.0f, -80.0f), 3, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Attack2", m_pFalseImg
			, Vector2(0.0f, 6159.0f), Vector2(705.0f, 595.0f)
			, Vector2(100.0f, -80.0f), 3, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Left_Attack3", m_pFalseImg
			, Vector2(0.0f, 6754.0f), Vector2(658.0f, 579.0f)
			, Vector2(30.0f, -90.0f), 5, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Attack3", m_pFalseImg
			, Vector2(0.0f, 7333.0f), Vector2(658.0f, 579.0f)
			, Vector2(-30.0f, -90.0f), 5, 0.1f);
		// 왼쪽에서 회수, 그다음껀 오른쪽에서 회수
		m_pFalseAttack->CreateAnimation(L"False_Left_Earthquake", m_pFalseImg
			, Vector2(0.0f, 12378.0f), Vector2(658.0f, 579.0f)
			, Vector2(30.0f, -90.0f), 2, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Earthquake", m_pFalseImg
			, Vector2(1316.0f, 12378.0f), Vector2(658.0f, 579.0f)
			, Vector2(-30.0f, -90.0f), 2, 0.1f);
		// Shock_Wave 1번 
		m_pFalseAttack->CreateAnimation(L"False_Left_Shock_Wave", m_pFalseImg
			, Vector2(582.0f, 4760.0f), Vector2(582.0f, 402.0f)
			, Vector2(150.0f, 0.0f), 5, 0.15f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Shock_Wave", m_pFalseImg
			, Vector2(582.0f, 5162.0f), Vector2(582.0f, 402.0f)
			, Vector2(-150.0f, 0.0f), 5, 0.15f);

		//Jump_Attack (점프해서 해머로 공격) (4피스)
		m_pFalseAttack->CreateAnimation(L"False_Left_Jump_Attack1", m_pFalseImg
			, Vector2(0.0f, 7912.0f), Vector2(713.0f, 424.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Jump_Attack1", m_pFalseImg
			, Vector2(0.0f, 8336.0f), Vector2(713.0f, 424.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Left_Jump_Attack2", m_pFalseImg
			, Vector2(0.0f, 8760.0f), Vector2(842.0f, 624.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Jump_Attack2", m_pFalseImg
			, Vector2(0.0f, 9384.0f), Vector2(842.0f, 624.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Left_Jump_Attack3", m_pFalseImg
			, Vector2(1684.0f, 8760.0f), Vector2(489.0f, 592.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Jump_Attack3", m_pFalseImg
			, Vector2(1684.0f, 9384.0f), Vector2(489.0f, 592.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Left_Jump_Attack4", m_pFalseImg
			, Vector2(0.0f, 10008.0f), Vector2(618.0f, 398.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Jump_Attack4", m_pFalseImg
			, Vector2(1236.0f, 10008.0f), Vector2(618.0f, 398.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);

		//Stun (그로기 상태) 2피스 + (본체 나오는것 까지) 2피스  // 총 4피스구성
		m_pFalseAttack->CreateAnimation(L"False_Left_Stun1", m_pFalseImg
			, Vector2(0.0f, 10406.0f), Vector2(765.0f, 518.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Stun1", m_pFalseImg
			, Vector2(0.0f, 10924.0f), Vector2(765.0f, 518.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Stun2", m_pFalseImg
			, Vector2(0.0f, 11442.0f), Vector2(419.0f, 468.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Left_Stun2", m_pFalseImg
			, Vector2(0.0f, 11910.0f), Vector2(419.0f, 468.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);

		m_pFalseAttack->CreateAnimation(L"False_Left_Head1", m_pFalseImg
			, Vector2(1676.0f, 11442.0f), Vector2(396.0f, 457.0f)
			, Vector2(-40.0f, -10.0f), 5, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Head1", m_pFalseImg
			, Vector2(1676.0f, 11910.0f), Vector2(396.0f, 457.0f)
			, Vector2(40.0f, -10.0f), 5, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Left_Head2", m_pFalseImg
			, Vector2(3257.0f, 11442.0f), Vector2(384.0f, 459.0f)
			, Vector2(0.0f, 0.0f), 1, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Head2", m_pFalseImg
			, Vector2(3257.0f, 11910.0f), Vector2(384.0f, 459.0f)
			, Vector2(0.0f, 0.0f), 1, 0.1f);

		//본체(얼굴)만 총 2피스 기본상태 얼굴 + 피격 상태 얼굴 + 몸체만(이펙트 애니메이션으로)
		m_pFalseEffect->CreateAnimation(L"False_Left_Main", m_pFalseImg
			, Vector2(2632.0f, 12378.0f), Vector2(118.0f, 120.0f)
			, Vector2(-160.0f, 160.0f), 5, 0.1f);
		m_pFalseEffect->CreateAnimation(L"False_Right_Main", m_pFalseImg
			, Vector2(2632.0f, 12498.0f), Vector2(118.0f, 120.0f)
			, Vector2(160.0f, 160.0f), 5, 0.1f);
		m_pFalseEffect->CreateAnimation(L"False_Left_Main_Hit", m_pFalseImg
			, Vector2(2632.0f, 12618.0f), Vector2(121.0f, 140.0f)
			, Vector2(-160.0f, 150.0f), 3, 0.1f);
		m_pFalseEffect->CreateAnimation(L"False_Right_Main_Hit", m_pFalseImg
			, Vector2(2632.0f, 12758.0f), Vector2(121.0f, 140.0f)
			, Vector2(160.0f, 150.0f), 3, 0.1f);
		m_pFalseEffect->CreateAnimation(L"Dumi", m_pFalseImg
			, Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f)
			, Vector2(0.0f, 0.0f), 1, 0.1f);

		m_pFalseAttack->CreateAnimation(L"False_Left_Body", m_pFalseImg
			, Vector2(2645.0f, 5933.0f), Vector2(347.0f, 259.0f)
			, Vector2(0.0f, 90.0f), 1, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Body", m_pFalseImg
			, Vector2(2645.0f, 6192.0f), Vector2(347.0f, 259.0f)
			, Vector2(0.0f, 90.0f), 1, 0.1f);


		//그로기 상태에서 다시 원래 상태로 돌아오는 거 1피스 + 다른거 조합
		m_pFalseAttack->CreateAnimation(L"False_Left_Recovery1", m_pFalseImg
			, Vector2(384.0f, 12957.0f), Vector2(396.0f, 457.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Recovery1", m_pFalseImg
			, Vector2(384.0f, 13414.0f), Vector2(396.0f, 457.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Left_Recovery2", m_pFalseImg
			, Vector2(0.0f, 13882.0f), Vector2(419.0f, 468.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Recovery2", m_pFalseImg
			, Vector2(0.0f, 14350.0f), Vector2(419.0f, 468.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Left_Recovery3", m_pFalseImg
			, Vector2(0.0f, 14818.0f), Vector2(656.0f, 585.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);
		m_pFalseAttack->CreateAnimation(L"False_Right_Recovery3", m_pFalseImg
			, Vector2(0.0f, 15403.0f), Vector2(656.0f, 585.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);

	}

	void False_Knight::SoundInitialize()
	{
		if (nullptr == m_sShock)
			m_sShock = Resources::Load<Sound>(L"False_Shock_S", L"..\\Resources\\Sound\\False_Shock_S.wav");
		if (nullptr == m_sSwing)
			m_sSwing = Resources::Load<Sound>(L"False_Swing_S", L"..\\Resources\\Sound\\False_Swing_S.wav");
		if (nullptr == m_sStrike)
			m_sStrike = Resources::Load<Sound>(L"False_Strike_S", L"..\\Resources\\Sound\\False_Strike_S.wav");
		if (nullptr == m_sJump)
			m_sJump = Resources::Load<Sound>(L"False_Jump_S", L"..\\Resources\\Sound\\False_Jump_S.wav");
		if (nullptr == m_sLand)
			m_sLand = Resources::Load<Sound>(L"False_Land_S", L"..\\Resources\\Sound\\False_Land_S.wav");
		if (nullptr == m_sJump_Attack)
			m_sJump_Attack = Resources::Load<Sound>(L"False_Jump_Attack_S", L"..\\Resources\\Sound\\False_Jump_Attack_S.wav");
		if (nullptr == m_sShoulder)
			m_sShoulder = Resources::Load<Sound>(L"False_Shoulder_S", L"..\\Resources\\Sound\\False_Shoulder_S.wav");
		if (nullptr == m_sEarthquake)
			m_sEarthquake = Resources::Load<Sound>(L"False_Earthquake_S", L"..\\Resources\\Sound\\False_Earthquake_S.wav");
		if (nullptr == m_sEarthquake1)
			m_sEarthquake1 = Resources::Load<Sound>(L"False_Earthquake_S1", L"..\\Resources\\Sound\\False_Earthquake_S1.wav");
		if (nullptr == m_sHit)
			m_sHit = Resources::Load<Sound>(L"False_Hit_S", L"..\\Resources\\Sound\\False_Hit_S.wav");
		if (nullptr == m_sStun)
			m_sStun = Resources::Load<Sound>(L"False_Stun_S", L"..\\Resources\\Sound\\False_Stun_S.wav");
		if (nullptr == m_sHead)
			m_sHead = Resources::Load<Sound>(L"False_Head_S", L"..\\Resources\\Sound\\False_Head_S.wav");




	}



	void False_Knight::Hammer_Attack()
	{
		if (m_tMonster.Progress == PROGRESS::start)
		{
			
			if (m_tMonster.Direction == DIRECTION::Left)
				m_pFalseAttack->Play(L"False_Left_Attack1", false);
			if (m_tMonster.Direction == DIRECTION::Right)
				m_pFalseAttack->Play(L"False_Right_Attack1", false);

			m_tMonster.Action = MONSTER_ACTION::Attack;
			m_tMonster.Progress = PROGRESS::step_1;
		}

		if (m_tMonster.Progress == PROGRESS::step_1)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.5)
			{
				m_sSwing->Play(false);
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pFalseAttack->Play(L"False_Left_Attack2", false);
					Attack();
				}
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pFalseAttack->Play(L"False_Right_Attack2", false);
					Attack();
				}

				m_tMonster.Progress = PROGRESS::step_2;
				s_fTimer = 0.f;
			}
		}

		if (m_tMonster.Progress == PROGRESS::step_2)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.3)
			{
				m_sStrike->Play(false);
				if (m_tMonster.Direction == DIRECTION::Left)
					m_pFalseAttack->Play(L"False_Left_Attack3", false);
				if (m_tMonster.Direction == DIRECTION::Right)
					m_pFalseAttack->Play(L"False_Right_Attack3", false);

				m_tMonster.Progress = PROGRESS::step_3;
				s_fTimer = 0.f;
			}
		}
		if (m_tMonster.Progress == PROGRESS::step_3)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.5)
			{
				if (m_tMonster.Direction == DIRECTION::Left)
					m_pFalseAttack->Play(L"False_Left_Standing", true);
				if (m_tMonster.Direction == DIRECTION::Right)
					m_pFalseAttack->Play(L"False_Right_Standing", true);

				m_tMonster.Progress = PROGRESS::start;
				m_tMonster.Skill = FALSE_SKILL::End;
				m_tMonster.Action = MONSTER_ACTION::End;
				s_fTimer = 0.f;
			}
		}
	}

	void False_Knight::Hammer_Earthquake(int number)
	{
		if (m_tMonster.Progress == PROGRESS::start)
		{
			m_sShock->Play(false);
			if (m_tMonster.Direction == DIRECTION::Left)
				m_pFalseAttack->Play(L"False_Left_Attack1", false);
			if (m_tMonster.Direction == DIRECTION::Right)
				m_pFalseAttack->Play(L"False_Right_Attack1", false);

			m_tMonster.Action = MONSTER_ACTION::Attack;
			m_tMonster.Progress = PROGRESS::step_1;
		}

		if (m_tMonster.Progress == PROGRESS::step_1)
		{
			s_fTimer += DT;
			if (s_fTimer >= 1.3)
			{
				m_tMonster.Progress = PROGRESS::step_2;
				s_fTimer = 0.f;
			}
		}
		
		if (m_tMonster.Progress == PROGRESS::step_2)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.2)
			{
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pFalseAttack->Play(L"False_Left_Attack2", false);
					m_sEarthquake->Play(false);
					Attack();
					
				}
				else if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pFalseAttack->Play(L"False_Right_Attack2", false);	
					m_sEarthquake1->Play(false);
					Attack();
				}

				
				m_tMonster.Progress = PROGRESS::step_3;
				s_fTimer = 0.f;
			}
		}

		//짝수 만큼 할거고 , 반복 
		if (m_tMonster.Progress == PROGRESS::step_3)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.3)
			{
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pFalseAttack->Play(L"False_Left_Earthquake", false);
					m_tMonster.Direction = DIRECTION::Right;
					Earthquak();
					m_sStrike->Play(false);
					Camera::SetShock(true);
				}
				//if (m_tMonster.Direction == DIRECTION::Right)
				else
				{
					m_pFalseAttack->Play(L"False_Right_Earthquake", false);
					m_tMonster.Direction = DIRECTION::Left;
					m_sStrike->Play(false);
					Earthquak();	
					Camera::SetShock(true);
				}

				m_tMonster.Progress = PROGRESS::step_2;
				++s_Attack_Repeat;
				s_fTimer = 0.f;
			}


			if (s_Attack_Repeat == number)
			{
				if (m_tMonster.Direction == DIRECTION::Left)
				{					
					m_tMonster.Direction = DIRECTION::Left;
					m_pFalseAttack->Play(L"False_Left_Standing", true);
				}
				//if (m_tMonster.Direction == DIRECTION::Right)
				else
				{					
					m_tMonster.Direction = DIRECTION::Right;
					m_pFalseAttack->Play(L"False_Right_Standing", true);
				}
				m_tMonster.Progress = PROGRESS::start;
				m_tMonster.Skill = FALSE_SKILL::End;
				m_tMonster.Action = MONSTER_ACTION::End;
				s_Attack_Repeat = 0;
			}
		}

	}

	void False_Knight::Hammer_Shock_Wave()
	{
		if (m_tMonster.Progress == PROGRESS::start)
		{
			m_sShock->Play(false);
			if (m_tMonster.Direction == DIRECTION::Left)
				m_pFalseAttack->Play(L"False_Left_Shock_Wave", true);
			if (m_tMonster.Direction == DIRECTION::Right)
				m_pFalseAttack->Play(L"False_Right_Shock_Wave", true);

			m_tMonster.Action = MONSTER_ACTION::Attack;
			m_tMonster.Progress = PROGRESS::step_1;
		}

		if (m_tMonster.Progress == PROGRESS::step_1)
		{
			s_fTimer += DT;
			if (s_fTimer >= 1.5)
			{
				m_sSwing->Play(false);
				if (m_tMonster.Direction == DIRECTION::Left)
					m_pFalseAttack->Play(L"False_Left_Attack2", false);		
				if (m_tMonster.Direction == DIRECTION::Right)
					m_pFalseAttack->Play(L"False_Right_Attack2", false);
				Attack();
				m_tMonster.Progress = PROGRESS::step_2;
				s_fTimer = 0.f;
			}
		}

		if (m_tMonster.Progress == PROGRESS::step_2)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.3)
			{
				m_sStrike->Play(false);
				m_Attack = ya::object::Instantiate<False_Attack>(eColliderLayer::Monster_Projecttile);
				m_Attack->SetPos(GetPos());
				m_Attack->SetSkill(False_Skill::Shock_Wave);
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pFalseAttack->Play(L"False_Left_Attack3", false);
					m_Attack->SetDir(Attack_Direction::Left);
				}
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pFalseAttack->Play(L"False_Right_Attack3", false);
					m_Attack->SetDir(Attack_Direction::Right);
				}
				m_tMonster.Progress = PROGRESS::step_3;
				s_fTimer = 0.f;
			}
		}
		if (m_tMonster.Progress == PROGRESS::step_3)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.5)
			{
				if (m_tMonster.Direction == DIRECTION::Left)
					m_pFalseAttack->Play(L"False_Left_Standing", true);
				if (m_tMonster.Direction == DIRECTION::Right)
					m_pFalseAttack->Play(L"False_Right_Standing", true);

				m_tMonster.Progress = PROGRESS::start;
				m_tMonster.Skill = FALSE_SKILL::End;
				m_tMonster.Action = MONSTER_ACTION::End;
				s_fTimer = 0.f;
			}
		}





	}

	void False_Knight::Shoulder_Attack()
	{
		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();

		WindowData Resolution = Application::GetInstance().GetWindowData();

		if (m_tMonster.Progress == PROGRESS::start)
		{			
			m_sShoulder->Play(false);
			//플레이어가 오른쪽에 있다.
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				m_pFalseAttack->Play(L"False_Right_Run1", false);
				m_tMonster.Direction = DIRECTION::Right;	
				m_pCollider->SetOffset(Vector2(120.f, 0.f));
			}
			// 플레이어가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				m_pFalseAttack->Play(L"False_Left_Run1", false);
				m_tMonster.Direction = DIRECTION::Left;
				m_pCollider->SetOffset(Vector2(-120.f, 0.f));
			}
			m_tMonster.Progress = PROGRESS::step_1;
			m_tMonster.Action = MONSTER_ACTION::Attack;
		}

		if (m_tMonster.Progress == PROGRESS::step_1)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.2f)
			{	
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pFalseAttack->Play(L"False_Left_Walk", true);
					
				}
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pFalseAttack->Play(L"False_Right_Walk", true);
				
				}
				m_vAttackPos = m_vTargetPos;
				m_tMonster.Progress = PROGRESS::step_2;				
				s_fTimer = 0.f;
			}
		}

		if (m_tMonster.Progress == PROGRESS::step_2)
		{
			if (m_tMonster.Direction == DIRECTION::Left)
			{
				if (m_vMovePos.x >= m_vAttackPos.x && !(m_bLeftWalkCheak))
					m_vMovePos.x -= 800.f * DT;
				else
				{
					m_pCollider->SetOffset(Vector2(0.f, 0.f));
					if (m_vTargetPos.x > m_vMovePos.x)
					{
						m_pFalseAttack->Play(L"False_Right_Turn", false);
						m_tMonster.Progress = PROGRESS::end;
					}
					else 
						m_tMonster.Progress = PROGRESS::end;
				}
			}
			if (m_tMonster.Direction == DIRECTION::Right)
			{
				if (m_vMovePos.x <= m_vAttackPos.x && !(m_bRightWalkCheak))
					m_vMovePos.x += 800.f * DT;
				else
				{
					m_pCollider->SetOffset(Vector2(0.f, 0.f));
					if (m_vTargetPos.x < m_vMovePos.x)
					{
						m_pFalseAttack->Play(L"False_Left_Turn", false);
						m_tMonster.Progress = PROGRESS::end;
					}
					else
					m_tMonster.Progress = PROGRESS::end;
				}
			}
			SetPos(m_vMovePos);

		}
		if (m_tMonster.Progress == PROGRESS::end)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.2)
			{
				if (m_tMonster.Direction == DIRECTION::Left)
					m_pFalseAttack->Play(L"False_Left_Standing", true);
				if (m_tMonster.Direction == DIRECTION::Right)
					m_pFalseAttack->Play(L"False_Right_Standing", true);

				m_tMonster.Progress = PROGRESS::start;
				m_tMonster.Skill = FALSE_SKILL::End;
				m_tMonster.Action = MONSTER_ACTION::End;
				s_fTimer = 0.f;
			}


		}

	}

	void False_Knight::Jump_Attack()
	{
		m_vMovePos = GetPos();


		if (m_tMonster.Progress == PROGRESS::start)
		{
			m_sJump_Attack->Play(false);
			if (m_tMonster.Direction == DIRECTION::Left)
				m_pFalseAttack->Play(L"False_Left_Jump1", false);
			if (m_tMonster.Direction == DIRECTION::Right)
				m_pFalseAttack->Play(L"False_Right_Jump1", false);

			m_tMonster.Action = MONSTER_ACTION::None;
			m_tMonster.Progress = PROGRESS::step_1;
		}

		if (m_tMonster.Progress == PROGRESS::step_1)
		{
			if (m_pPlayer != nullptr)
				m_vTargetPos = m_pPlayer->GetPos();
			m_vAttackPos = m_vTargetPos;
			s_fTimer += DT;
			if (s_fTimer >= 0.75)
			{
				m_sJump->Play(false);
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pFalseAttack->Play(L"False_Left_Jump_Attack1", false);
				}
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pFalseAttack->Play(L"False_Right_Jump_Attack1", false);
				}
				m_tMonster.Progress = PROGRESS::step_2;
				m_vFixPos = GetPos();
				s_fTimer = 0.f;
			}
		}

		if (m_tMonster.Progress == PROGRESS::step_2)
		{
			if (m_tMonster.Direction == DIRECTION::Left)
			{
				Vector2 mPOS = {};
				//mPOS.x = ((m_vAttackPos.x + m_vMovePos.x) / 2 + (distance * 2) - m_vMovePos.x);
				mPOS.x = ((m_vAttackPos.x + m_vFixPos.x) / 2 - m_vMovePos.x);
				mPOS.y = (m_vAttackPos.y - 550) - m_vMovePos.y;
				mPOS.Normalize();
				if (m_vMovePos.y >= m_vAttackPos.y - 550 && !(m_bLeftWalkCheak))
				{
					m_vMovePos.x += mPOS.x * 850 * DT;
					m_vMovePos.y += mPOS.y * 850 * DT;
				}
				else
				{
					m_sSwing->Play(false);
					m_pFalseAttack->Play(L"False_Left_Jump_Attack2", false);
					m_tMonster.Progress = PROGRESS::step_3;
				}
			}
			if (m_tMonster.Direction == DIRECTION::Right)
			{
				Vector2 mPOS = {};
				//mPOS.x = ((m_vAttackPos.x + m_vMovePos.x) / 2 - (distance * 2) - m_vMovePos.x);
				mPOS.x = ((m_vAttackPos.x + m_vFixPos.x) / 2 - m_vMovePos.x);
				mPOS.y = (m_vAttackPos.y - 550) - m_vMovePos.y;
				mPOS.Normalize();
				if (m_vMovePos.y >= m_vAttackPos.y - 550 && !(m_bRightWalkCheak))
				{
					m_vMovePos.x += mPOS.x * 850 * DT;
					m_vMovePos.y += mPOS.y * 850 * DT;
				}
				else
				{
					m_sSwing->Play(false);
					m_pFalseAttack->Play(L"False_Right_Jump_Attack2", false);
					m_tMonster.Progress = PROGRESS::step_3;
				}
			}


			SetPos(m_vMovePos);
		}

		if (m_tMonster.Progress == PROGRESS::step_3)
		{

			if (m_tMonster.Direction == DIRECTION::Left)
			{
				Vector2 mPOS = {};
				mPOS.x = (m_vAttackPos.x + 110) - m_vMovePos.x;
				mPOS.y = m_vAttackPos.y - m_vMovePos.y;
				mPOS.Normalize();
				if (m_vMovePos.y <= m_vAttackPos.y - 110 && !(m_bLeftWalkCheak))
				{
					m_vMovePos.x += mPOS.x * 1000 * DT;
					m_vMovePos.y += mPOS.y * 1000 * DT;
				}
				else
				{
					m_sStrike->Play(false);
					m_pFalseAttack->Play(L"False_Left_Jump_Attack3", false);
					m_tMonster.Progress = PROGRESS::step_4;
					Attack();
				}
			}
			if (m_tMonster.Direction == DIRECTION::Right)
			{
				Vector2 mPOS = {};
				mPOS.x = (m_vAttackPos.x - 110) - m_vMovePos.x;
				mPOS.y = m_vAttackPos.y - m_vMovePos.y;
				mPOS.Normalize();

				if (m_vMovePos.y <= m_vAttackPos.y - 110 && !(m_bRightWalkCheak))
				{
					m_vMovePos.x += mPOS.x * 1000 * DT;
					m_vMovePos.y += mPOS.y * 1000 * DT;
				}
				else
				{
					m_sStrike->Play(false);
					m_pFalseAttack->Play(L"False_Right_Jump_Attack3", false);
					m_tMonster.Progress = PROGRESS::step_4;
					Attack();
				}

			}
			SetPos(m_vMovePos);
		}
		if (m_tMonster.Progress == PROGRESS::step_4)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.2)
			{
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pFalseAttack->Play(L"False_Left_Jump_Attack4", false);
				}

				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pFalseAttack->Play(L"False_Right_Jump_Attack4", false);
				}
				s_fTimer = 0.f;
				m_tMonster.Progress = PROGRESS::end;
			}


		}


		if (m_tMonster.Progress == PROGRESS::end)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.2)
			{
				if (m_vTargetPos.x > m_vMovePos.x)
					m_pFalseAttack->Play(L"False_Right_Standing", true);
				if (m_vTargetPos.x < m_vMovePos.x)
					m_pFalseAttack->Play(L"False_Left_Standing", true);

				m_tMonster.Progress = PROGRESS::start;
				m_tMonster.Skill = FALSE_SKILL::End;
				m_tMonster.Action = MONSTER_ACTION::End;
				s_fTimer = 0.f;
			}
		}
	}



	void False_Knight::False_Turn()
	{
		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();

		if(m_tMonster.Progress == PROGRESS::start)
		{
			// 플레이어가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				m_pFalseAttack->Play(L"False_Left_Turn", false);
				m_tMonster.Direction = DIRECTION::Left;
			}
			//플레이어가 오른쪽에 있다.
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				m_pFalseAttack->Play(L"False_Right_Turn", false);
				m_tMonster.Direction = DIRECTION::Right;
			}

			m_tMonster.Progress = PROGRESS::step_1;
		}

		if (m_tMonster.Progress == PROGRESS::step_1)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.2f)
			{
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pFalseAttack->Play(L"False_Left_Standing", true);
				}
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pFalseAttack->Play(L"False_Right_Standing", true);
				}				
				m_tMonster.Progress = PROGRESS::start;				
				s_fTimer = 0.f;
			}

		}

	}

	void False_Knight::False_Walk(float distance)
	{

		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();

		WindowData Resolution = Application::GetInstance().GetWindowData();

		if (m_tMonster.Progress == PROGRESS::start)
		{
			//플레이어가 오른쪽에 있다.
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				m_pFalseAttack->Play(L"False_Right_Run1", false);
				m_tMonster.Direction = DIRECTION::Right;
				m_tMonster.Progress = PROGRESS::step_1;
			}
			// 플레이어가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				m_pFalseAttack->Play(L"False_Left_Run1", false);
				m_tMonster.Direction = DIRECTION::Left;
				m_tMonster.Progress = PROGRESS::step_1;
			}

			m_tMonster.Action = MONSTER_ACTION::Walk;
		}
		if (m_tMonster.Progress == PROGRESS::step_1)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.2f)
			{
				if (m_vTargetPos.x > m_vMovePos.x)
				{
					m_pFalseAttack->Play(L"False_Right_Walk", true);
					m_tMonster.Direction = DIRECTION::Right;
					m_tMonster.Progress = PROGRESS::step_2;
				}				
				if (m_vTargetPos.x < m_vMovePos.x)
				{
					m_pFalseAttack->Play(L"False_Left_Walk", true);
					m_tMonster.Direction = DIRECTION::Left;
					m_tMonster.Progress = PROGRESS::step_2;
				}
				s_fTimer = 0.f;
			}
		}



		if (m_tMonster.Progress == PROGRESS::step_2)
		{
			if (m_tMonster.Direction == DIRECTION::Left)
			{
				//if (m_vMovePos.x > (Resolution.width / Resolution.width) && abs(m_vMovePos.x - m_vTargetPos.x) > distance)
				if (!(m_bLeftWalkCheak) && abs(m_vMovePos.x - m_vTargetPos.x) > distance)
				{
					m_vMovePos.x -= 400.f * DT;
				}
				else
				{
					m_tMonster.Skill = (FALSE_SKILL)s_iRandom;
					m_tMonster.Action = MONSTER_ACTION::None;
					m_tMonster.Progress = PROGRESS::start;
					if (m_vTargetPos.x > m_vMovePos.x)
						m_pFalseAttack->Play(L"False_Right_Standing", true);
					if (m_vTargetPos.x < m_vMovePos.x)
						m_pFalseAttack->Play(L"False_Left_Standing", true);
				}
			}
			if (m_tMonster.Direction == DIRECTION::Right)
			{
				//if (m_vMovePos.x <= (Resolution.width) && abs(m_vMovePos.x - m_vTargetPos.x) > distance)
				if (!(m_bRightWalkCheak) && abs(m_vMovePos.x - m_vTargetPos.x) > distance)
				{
					m_vMovePos.x += 400.f * DT;
				}
				else
				{
					m_tMonster.Skill = (FALSE_SKILL)s_iRandom;
					m_tMonster.Action = MONSTER_ACTION::None;
					m_tMonster.Progress = PROGRESS::start;
					if (m_vTargetPos.x > m_vMovePos.x)
						m_pFalseAttack->Play(L"False_Right_Standing", true);
					if (m_vTargetPos.x < m_vMovePos.x)
						m_pFalseAttack->Play(L"False_Left_Standing", true);
				}
			}
			SetPos(m_vMovePos);
		}


	}

	void False_Knight::False_Jump(float distance)
	{

		m_vMovePos = GetPos();
	

		if (m_tMonster.Progress == PROGRESS::start)
		{			
			if (m_tMonster.Direction == DIRECTION::Left)
				m_pFalseAttack->Play(L"False_Left_Jump1", false);
			if (m_tMonster.Direction == DIRECTION::Right)
				m_pFalseAttack->Play(L"False_Right_Jump1", false);

			m_tMonster.Action = MONSTER_ACTION::None;
			m_tMonster.Progress = PROGRESS::step_1;
		}
			
		if (m_tMonster.Progress == PROGRESS::step_1)
		{
			if (m_pPlayer != nullptr)
				m_vTargetPos = m_pPlayer->GetPos();
			m_vAttackPos = m_vTargetPos;
			s_fTimer += DT;
			if (s_fTimer >= 0.75)
			{		
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pFalseAttack->Play(L"False_Left_Jump2", false);
				}
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pFalseAttack->Play(L"False_Right_Jump2", false);
				}
				m_sJump->Play(false);
				m_tMonster.Progress = PROGRESS::step_2;	
				m_vFixPos = GetPos();
				s_fTimer = 0.f;
			}
		}

		if (m_tMonster.Progress == PROGRESS::step_2)
		{
			if (m_tMonster.Direction == DIRECTION::Left)
			{
				Vector2 mPOS = {};
				//mPOS.x = ((m_vAttackPos.x + m_vMovePos.x) / 2 + (distance * 2) - m_vMovePos.x);
				mPOS.x = ((m_vAttackPos.x + m_vFixPos.x) / 2 - m_vMovePos.x);
				mPOS.y = (m_vAttackPos.y - 550) - m_vMovePos.y;
				mPOS.Normalize();
				if (m_vMovePos.y >= m_vAttackPos.y - 550 && !(m_bLeftWalkCheak))
				{
					m_vMovePos.x += mPOS.x * 850 * DT;
					m_vMovePos.y += mPOS.y * 850 * DT;
				}
				else
				{
					m_pFalseAttack->Play(L"False_Left_Jump3", false);
					m_tMonster.Progress = PROGRESS::step_3;
				}
			}
			if (m_tMonster.Direction == DIRECTION::Right)
			{
				Vector2 mPOS = {};
				//mPOS.x = ((m_vAttackPos.x + m_vMovePos.x) / 2 - (distance * 2) - m_vMovePos.x);
				mPOS.x = ((m_vAttackPos.x + m_vFixPos.x) / 2 - m_vMovePos.x);
				mPOS.y = (m_vAttackPos.y - 550) - m_vMovePos.y;
				mPOS.Normalize();
				if (m_vMovePos.y >= m_vAttackPos.y - 550 && !(m_bRightWalkCheak))
				{
					m_vMovePos.x += mPOS.x * 850 * DT;
					m_vMovePos.y += mPOS.y * 850 * DT;
				}
				else
				{
					m_pFalseAttack->Play(L"False_Right_Jump3", false);
					m_tMonster.Progress = PROGRESS::step_3;
				}
			}


			SetPos(m_vMovePos);
		}

		if (m_tMonster.Progress == PROGRESS::step_3)
		{
			
			if (m_tMonster.Direction == DIRECTION::Left)
			{
				Vector2 mPOS = {};
				mPOS.x = (m_vAttackPos.x + distance) - m_vMovePos.x;
				mPOS.y = m_vAttackPos.y - m_vMovePos.y;
				mPOS.Normalize();
				if (m_vMovePos.y <= m_vAttackPos.y - 110 && !(m_bLeftWalkCheak))
				{
					m_vMovePos.x += mPOS.x * 1000 * DT;
					m_vMovePos.y += mPOS.y * 1000 * DT;
				}
				else
				{
					m_sLand->Play(false);
					m_tMonster.Progress = PROGRESS::end;
				}
			}
			if (m_tMonster.Direction == DIRECTION::Right)
			{
					Vector2 mPOS = {};
					mPOS.x = (m_vAttackPos.x - distance) - m_vMovePos.x;
					mPOS.y = m_vAttackPos.y - m_vMovePos.y;
					mPOS.Normalize();

					if (m_vMovePos.y <= m_vAttackPos.y - 110 && !(m_bRightWalkCheak))
					{
						m_vMovePos.x += mPOS.x * 1000 * DT;
						m_vMovePos.y += mPOS.y * 1000 * DT;
					}
					else
					{
						m_sLand->Play(false);
						m_tMonster.Progress = PROGRESS::end;
					}

			}
			SetPos(m_vMovePos);
		}

		if (m_tMonster.Progress == PROGRESS::end)
		{
			m_tMonster.Skill = (FALSE_SKILL)s_iRandom;
			m_tMonster.Action = MONSTER_ACTION::None;
			m_tMonster.Progress = PROGRESS::start;
			if (m_vTargetPos.x > m_vMovePos.x)
				m_pFalseAttack->Play(L"False_Right_Standing", true);
			if (m_vTargetPos.x < m_vMovePos.x)
				m_pFalseAttack->Play(L"False_Left_Standing", true);
		}

	}

	void False_Knight::False_BackJump(float distance)
	{
		m_vMovePos = GetPos();


		if (m_tMonster.Progress == PROGRESS::start)
		{
			if (m_tMonster.Direction == DIRECTION::Left)
				m_pFalseAttack->Play(L"False_Left_Jump1", false);
			if (m_tMonster.Direction == DIRECTION::Right)
				m_pFalseAttack->Play(L"False_Right_Jump1", false);

			m_tMonster.Action = MONSTER_ACTION::None;
			m_tMonster.Progress = PROGRESS::step_1;
		}

		if (m_tMonster.Progress == PROGRESS::step_1)
		{
			if (m_pPlayer != nullptr)
				m_vTargetPos = m_pPlayer->GetPos();
			m_vAttackPos = m_vTargetPos;
			s_fTimer += DT;
			if (s_fTimer >= 0.75)
			{
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pFalseAttack->Play(L"False_Left_Jump2", false);
					m_vAttackPos.x += distance;
				}
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pFalseAttack->Play(L"False_Right_Jump2", false);
					m_vAttackPos.x -= distance;
				}
				m_tMonster.Progress = PROGRESS::step_2;
				m_vFixPos = GetPos();
				s_fTimer = 0.f;
			}
		}

		if (m_tMonster.Progress == PROGRESS::step_2)
		{
			if (m_tMonster.Direction == DIRECTION::Left)
			{
				Vector2 mPOS = {};
				//mPOS.x = ((m_vAttackPos.x + m_vMovePos.x) / 2 + (distance * 2) - m_vMovePos.x);
				mPOS.x = ((m_vAttackPos.x + m_vFixPos.x) / 2 - m_vMovePos.x);
				mPOS.y = (m_vAttackPos.y - 550) - m_vMovePos.y;
				mPOS.Normalize();
				if (m_vMovePos.y >= m_vAttackPos.y - 550 && !(m_bLeftWalkCheak))
				{
					m_vMovePos.x += mPOS.x * 850 * DT;
					m_vMovePos.y += mPOS.y * 850 * DT;
				}
				else
				{
					m_pFalseAttack->Play(L"False_Left_Jump3", false);
					m_tMonster.Progress = PROGRESS::step_3;
				}
			}
			if (m_tMonster.Direction == DIRECTION::Right)
			{
				Vector2 mPOS = {};
				//mPOS.x = ((m_vAttackPos.x + m_vMovePos.x) / 2 - (distance * 2) - m_vMovePos.x);
				mPOS.x = ((m_vAttackPos.x + m_vFixPos.x) / 2 - m_vMovePos.x);
				mPOS.y = (m_vAttackPos.y - 550) - m_vMovePos.y;
				mPOS.Normalize();
				if (m_vMovePos.y >= m_vAttackPos.y - 550 && !(m_bRightWalkCheak))
				{
					m_vMovePos.x += mPOS.x * 850 * DT;
					m_vMovePos.y += mPOS.y * 850 * DT;
				}
				else
				{
					m_pFalseAttack->Play(L"False_Right_Jump3", false);
					m_tMonster.Progress = PROGRESS::step_3;
				}
			}


			SetPos(m_vMovePos);
		}

		if (m_tMonster.Progress == PROGRESS::step_3)
		{

			if (m_tMonster.Direction == DIRECTION::Left)
			{
				Vector2 mPOS = {};
				mPOS.x = (m_vAttackPos.x) - m_vMovePos.x;
				mPOS.y = m_vAttackPos.y - m_vMovePos.y;
				mPOS.Normalize();
				if (m_vMovePos.y <= m_vAttackPos.y - 110 && !(m_bLeftWalkCheak))
				{
					m_vMovePos.x += mPOS.x * 1000 * DT;
					m_vMovePos.y += mPOS.y * 1000 * DT;
				}
				else
				{
					m_tMonster.Progress = PROGRESS::end;
				}
			}
			if (m_tMonster.Direction == DIRECTION::Right)
			{
				Vector2 mPOS = {};
				mPOS.x = (m_vAttackPos.x ) - m_vMovePos.x;
				mPOS.y = m_vAttackPos.y - m_vMovePos.y;
				mPOS.Normalize();

				if (m_vMovePos.y <= m_vAttackPos.y - 110 && !(m_bRightWalkCheak))
				{
					m_vMovePos.x += mPOS.x * 1000 * DT;
					m_vMovePos.y += mPOS.y * 1000 * DT;
				}
				else
				{
					m_tMonster.Progress = PROGRESS::end;
				}

			}
			SetPos(m_vMovePos);
		}

		if (m_tMonster.Progress == PROGRESS::end)
		{
			m_tMonster.Skill = (FALSE_SKILL)s_iRandom;
			m_tMonster.Action = MONSTER_ACTION::None;
			m_tMonster.Progress = PROGRESS::start;
			if (m_vTargetPos.x > m_vMovePos.x)
				m_pFalseAttack->Play(L"False_Right_Standing", true);
			if (m_vTargetPos.x < m_vMovePos.x)
				m_pFalseAttack->Play(L"False_Left_Standing", true);
		}


	}

	void False_Knight::Earthquak()
	{
		m_Attack = ya::object::Instantiate<False_Attack>(eColliderLayer::Monster_Projecttile);
		//(최대 - 최소 + 1) + 최소
		//int ran = rand() % (1500 - 400 + 1) + 400;
		int Col = rand() % ((1920 / 2 - 30) - 50 + 1) + 50;
		if (Col <= (int)(1920 / 2 - 100) / 2)
			Col *= -1;

		m_Attack->SetPos(Vector2(GetPos().x + Col, GetPos().y - 600));
		m_Attack->SetSkill(False_Skill::Earthquak);
	}

	void False_Knight::Attack()
	{
		if (m_tMonster.Direction == DIRECTION::Left)
		{			
			m_Attack = ya::object::Instantiate<False_Attack>(eColliderLayer::Monster_Projecttile);
			m_Attack->SetSkill(False_Skill::Attack);
			m_Attack->SetDir(Attack_Direction::Left);
			m_Attack->SetPos(GetPos());
		}
		if (m_tMonster.Direction == DIRECTION::Right)
		{			
			m_Attack = ya::object::Instantiate<False_Attack>(eColliderLayer::Monster_Projecttile);
			m_Attack->SetSkill(False_Skill::Attack);
			m_Attack->SetDir(Attack_Direction::Right);
			m_Attack->SetPos(GetPos());
		}

	}


	bool False_Knight::TargetPlayer()
	{

		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();

	
		//플레이어가 오른쪽에 있다.
		if (m_vTargetPos.x > m_vMovePos.x)
		{
			if (m_tMonster.Direction == DIRECTION::Right)
				return true;
			if (m_tMonster.Direction == DIRECTION::Left)
				return false;
		}
		// 플레이어가 왼쪽에 있다
		if (m_vTargetPos.x < m_vMovePos.x)
		{
			if (m_tMonster.Direction == DIRECTION::Right)
				return false;
			if (m_tMonster.Direction == DIRECTION::Left)
				return true;
		}

		return true;
	}

	void False_Knight::Stun()
	{

		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();

		
		if(m_tMonster.Progress == PROGRESS::start)
		{
			m_sStun->Play(false);
			CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, false);
			CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, false);
			//플레이어가 오른쪽에 있다.
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				m_pFalseAttack->Play(L"False_Left_Stun1", false);
				m_tMonster.Direction = DIRECTION::Right;				
			}
			// 플레이어가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				m_pFalseAttack->Play(L"False_Right_Stun1", false);
				m_tMonster.Direction = DIRECTION::Left;
			}

			m_vAttackPos = GetPos();
			m_tMonster.Progress = PROGRESS::step_1;
		}

		if (m_tMonster.Progress == PROGRESS::step_1)
		{

			if(m_tMonster.Direction == DIRECTION::Left)
			{
				if (m_vMovePos.x < m_vAttackPos.x + 200 && !(m_bRightWalkCheak))
					m_vMovePos.x += 400.f * DT;
				else
				{
					m_pFalseAttack->Play(L"False_Right_Stun2", false);
					m_tMonster.Progress = PROGRESS::step_2;
				}
			}
			if(m_tMonster.Direction == DIRECTION::Right)
			{ 
				if (m_vMovePos.x > m_vAttackPos.x - 200 && !(m_bLeftWalkCheak))
					m_vMovePos.x -= 400.f * DT;
				else
				{
					m_pFalseAttack->Play(L"False_Left_Stun2", false);
					m_tMonster.Progress = PROGRESS::step_2;
				}
			}
			SetPos(m_vMovePos);
		}

		if (m_tMonster.Progress == PROGRESS::step_2)
		{
			s_fTimer += DT;
			if (s_fTimer >= 1.0)
			{
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pFalseAttack->Play(L"False_Left_Head1", false);
				}
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pFalseAttack->Play(L"False_Right_Head1", false);
				}
				m_tMonster.Progress = PROGRESS::step_3;
				s_fTimer = 0.f;
			}
		}

		if (m_tMonster.Progress == PROGRESS::step_3)
		{

			s_fTimer += DT;
			if (s_fTimer >= 0.5)
			{
				
				CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, true);
				m_pCollider->SetScale(Vector2(120.f, 140.f));
				
				if (m_tMonster.Direction == DIRECTION::Left)
				{					
					m_pCollider->SetOffset(Vector2(-160.f, 150.f));
					m_pFalseAttack->Play(L"False_Left_Body", false);
					m_pFalseEffect->Play(L"False_Left_Main", true);
				}
				if (m_tMonster.Direction == DIRECTION::Right)
				{					
					m_pCollider->SetOffset(Vector2(160.f, 150.f));
					m_pFalseAttack->Play(L"False_Right_Body", false);
					m_pFalseEffect->Play(L"False_Right_Main", true);
				}
				m_tMonster.Progress = PROGRESS::step_4;

				s_fTimer = 0.f;
			}
		}
		if (m_tMonster.Progress == PROGRESS::step_4)
		{			
			if (m_StunHp > 0 && m_tMonster.Hit)
			{
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pFalseEffect->Play(L"False_Left_Main_Hit", false);
					m_tMonster.Progress = PROGRESS::step_3;
				}
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pFalseEffect->Play(L"False_Right_Main_Hit", false);
					m_tMonster.Progress = PROGRESS::step_3;
				}
			}

			else if(m_StunHp <= 0)
			{
				CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, false);
	
				m_pCollider->SetScale(Vector2(400.f, 400.f));
				m_pCollider->SetOffset(Vector2(0.f, 0.f));
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pFalseEffect->Play(L"Dumi", false);
					m_pFalseAttack->Play(L"False_Right_Recovery1", false);
				
				}
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pFalseEffect->Play(L"Dumi", false);
					m_pFalseAttack->Play(L"False_Left_Recovery1", false);
				}
					m_tMonster.Progress = PROGRESS::step_5;

			}
		}

		if (m_tMonster.Progress == PROGRESS::step_5)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.4)
			{
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pFalseAttack->Play(L"False_Right_Recovery2", false);
				}
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pFalseAttack->Play(L"False_Left_Recovery2", false);
				}
				m_tMonster.Progress = PROGRESS::step_6;
				m_vMovePos = GetPos();
				m_vAttackPos = GetPos();
				s_fTimer = 0.f;
			}
		}

		if (m_tMonster.Progress == PROGRESS::step_6)
		{
			if (m_tMonster.Direction == DIRECTION::Left)
			{
				if (m_vMovePos.x < m_vAttackPos.x + 150 && !(m_bRightWalkCheak))
					m_vMovePos.x += 400.f * DT;
				else
				{
					m_pFalseAttack->Play(L"False_Right_Recovery3", false);
					m_tMonster.Progress = PROGRESS::end;
				}
			}
			if (m_tMonster.Direction == DIRECTION::Right)
			{
				if (m_vMovePos.x > m_vAttackPos.x - 150 && !(m_bLeftWalkCheak))
					m_vMovePos.x -= 400.f * DT;
				else
				{
					m_pFalseAttack->Play(L"False_Left_Recovery3", false);
					m_tMonster.Progress = PROGRESS::end;
				}
			}
			SetPos(m_vMovePos);
		}
		if (m_tMonster.Progress == PROGRESS::end)
		{
			CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, true);
			CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, true);
			s_fTimer += DT;
			if (s_fTimer >= 0.3)
			{
				if (m_tMonster.Direction == DIRECTION::Left)
					m_pFalseAttack->Play(L"False_Left_Standing", true);
				if (m_tMonster.Direction == DIRECTION::Right)
					m_pFalseAttack->Play(L"False_Right_Standing", true);

				m_tMonster.Progress = PROGRESS::start;
				m_tMonster.Skill = FALSE_SKILL::End;
				m_tMonster.Action = MONSTER_ACTION::End;
				s_fTimer = 0.f;
			}



		}

	}

	void False_Knight::False_Death()
	{

		Rigidbody* rigidbody = GetComponent<Rigidbody>();
		rigidbody->SetGround(true);


		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();


		if (m_tMonster.Progress == PROGRESS::start)
		{
			m_sStun->Play(false);
			m_pCollider->SetScale(Vector2(0.f, 0.f));
			//플레이어가 오른쪽에 있다.
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				m_pFalseAttack->Play(L"False_Left_Stun1", false);
				m_tMonster.Direction = DIRECTION::Right;
			}
			// 플레이어가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				m_pFalseAttack->Play(L"False_Right_Stun1", false);
				m_tMonster.Direction = DIRECTION::Left;
			}

			m_vAttackPos = GetPos();
			m_tMonster.Progress = PROGRESS::step_1;
		}

		if (m_tMonster.Progress == PROGRESS::step_1)
		{

			if (m_tMonster.Direction == DIRECTION::Left)
			{
				if (m_vMovePos.x < m_vAttackPos.x + 200 && !(m_bRightWalkCheak))
					m_vMovePos.x += 400.f * DT;
				else
				{
					m_pFalseAttack->Play(L"False_Right_Stun2", false);
					m_tMonster.Progress = PROGRESS::step_2;
				}
			}
			if (m_tMonster.Direction == DIRECTION::Right)
			{
				if (m_vMovePos.x > m_vAttackPos.x - 200 && !(m_bLeftWalkCheak))
					m_vMovePos.x -= 400.f * DT;
				else
				{
					m_pFalseAttack->Play(L"False_Left_Stun2", false);
					m_tMonster.Progress = PROGRESS::step_2;
				}
			}
			SetPos(m_vMovePos);
		}

		if (m_tMonster.Progress == PROGRESS::step_2)
		{
			s_fTimer += DT;
			if (s_fTimer >= 2.0)
			{
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pFalseAttack->Play(L"False_Left_Head1", false);
				}
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pFalseAttack->Play(L"False_Right_Head1", false);
				}
				m_tMonster.Progress = PROGRESS::step_3;
				s_fTimer = 0.f;
			}
		}

		if (m_tMonster.Progress == PROGRESS::step_3)
		{

			s_fTimer += DT;
			if (s_fTimer >= 1.0)
			{

				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pCollider->SetOffset(Vector2(-160.f, 150.f));
					m_pFalseAttack->Play(L"False_Left_Body", false);
					m_pFalseEffect->Play(L"False_Left_Main", true);
				}
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pCollider->SetOffset(Vector2(160.f, 150.f));
					m_pFalseAttack->Play(L"False_Right_Body", false);
					m_pFalseEffect->Play(L"False_Right_Main", true);
				}
				CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, true);
				CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, true);
				False_Potar* false_knight = ya::object::Instantiate<False_Potar>(eColliderLayer::GameObject);
				Vector2 Pos = GetPos();
				Pos.y -= 50;
				false_knight->SetPos(Pos);
				m_tMonster.Progress = PROGRESS::step_4;

				s_fTimer = 0.f;
			}
		}



	}

}