
#include "yaHornet.h"
#include "yaImage.h"
#include "yaTime.h"
#include "yaResources.h"
#include "yaAnimator.h"
#include "yaCollider.h"
#include "yaPlayer.h"
#include "yaHornet_Attack.h"
#include "yaCollisionManager.h"
#include "yaTime.h"
#include "yaCamera.h"
#include "yaRigidbody.h"
#include "yaApplication.h"
#include "yaCamera.h"
#include "yaPlayer_Attack.h"
#include "yaSound.h"
#include <Time.h>



#include "yaObject.h"

namespace ya
{
	float Hornet::s_fTimer = 0.f;
	int Hornet::s_iCheak = 0;
	int Hornet::s_iRandom = 0;
	int Hornet::m_iPhase = 4;
	bool Hornet::s_bStart = false;

	Hornet::Hornet()
		: m_pHornet1Img(nullptr)
		, m_pHornet2Img(nullptr)
		, m_pHornet3Img(nullptr)
		, m_pHornetEffectImg(nullptr)
		, m_pHornetEndImg(nullptr)
		, m_pHornetAni(nullptr)
		, m_pHornetAniEffect(nullptr)
		, m_pPlayer(nullptr)
		, m_Attack(nullptr)
		, m_pStart(nullptr)
		, m_pLeftEvade(nullptr)
		, m_pRightEvade(nullptr)
		, m_pWalk(nullptr)
		, m_pG_Dash(nullptr)
		, m_pDash(nullptr)
		, m_pSphere(nullptr)
		, m_pSphere1(nullptr)
		, m_pA_Sphere(nullptr)
		, m_pJump(nullptr)
		, m_pS_Jump(nullptr)
		, m_pJump_Effect(nullptr)
		, m_pLand(nullptr)
		, m_pSpear(nullptr)
		, m_pSpear1(nullptr)
		, m_pSpear2(nullptr)
		, m_pTrap(nullptr)
		, m_pTrap1(nullptr)
		, m_pCounter(nullptr)
		, m_pCounter1(nullptr)
		, m_pStun(nullptr)
		, m_pDeath(nullptr)
		, m_pHit(nullptr)
		, m_bTarget(false)
		, m_bChange(false)
		, m_bPlay(false)
		, m_bHornet_Death(false)
		, m_bCounter(false)
		, m_vTargetPos(Vector2::Zero)
		, m_vFixPos(Vector2::Zero)
		, m_Hp(30)
	{
		SetName(L"Hornet");
		SetPos({ 500.0f, 500.0f });
		SetScale({ 1.0f, 1.0f });

		m_pHornet1Img = Resources::Load<Texture>(L"Hornet1", L"..\\Resources\\Image\\Hornet1.bmp");
		m_pHornet2Img = Resources::Load<Texture>(L"Hornet2", L"..\\Resources\\Image\\Hornet2.bmp");
		m_pHornet3Img = Resources::Load<Texture>(L"Hornet3", L"..\\Resources\\Image\\Hornet3.bmp");
		m_pHornetEffectImg = Resources::Load<Texture>(L"Hornet_Effect", L"..\\Resources\\Image\\Hornet_Effect.bmp");
		m_pHornetEndImg = Resources::Load<Texture>(L"Hornet_End", L"..\\Resources\\Image\\Hornet_End.bmp");
		


		m_tMonster.Direction = DIRECTION::Right;
		m_tMonster.HAction = HornetAction::None;
		
		//랜덤 함수 사용할때 의미 없는 함수 한번 사용해줘야 하는거같음...
		srand((UINT)time(nullptr));
		int dummy = rand();

		m_pHornetAni = new Animator();
		m_pHornetAniEffect = new Animator();

		m_pMonsterSize = new Collider();
		AddComponent(m_pMonsterSize);
		m_pMonsterSize->SetScale(Vector2(180.0f, 220.0f));





		AnimationInitialize();
		SoundInitialize();

		
		
		//m_pHornetAniEffect->Play(L"Hornet_Last_Effect", true);
		m_pHornetAni->Play(L"Hornet_Left_Standing", true);
		m_tMonster.HIA = HornetAI::End;
		m_tMonster.TarGet = Cheak::None;
		

		AddComponent(m_pHornetAni);
		AddComponent(m_pHornetAniEffect);
		m_vFixPos = GetPos();
		AddComponent<Rigidbody>();
		Rigidbody* rigidbody = GetComponent<Rigidbody>();
		rigidbody->SetGround(true);

	}

	Hornet::~Hornet()
	{
	}

	void Hornet::Tick()
	{

		//G_Dash_Attack();
		//Sphere_Attack();
		//Jump_Sphere_Attack();		
		//Spear_Attack();
		//Hornet_Jump();
		//Evade();
		//HornetWalk(500);
		//Hornet_Trap();
		//Hornet_Defence();
		//Hornet_Stun();
		//Hornet_End();


		if (s_bStart == false)
		{
			s_fTimer += DT;
			if (s_fTimer >= 4.5 && s_fTimer <= 4.51)
			{
				Rigidbody* rigidbody = GetComponent<Rigidbody>();
				rigidbody->SetGround(false);
				m_pStart->Play(false);
				m_pHornetAni->Play(L"Hornet_Left_Play", true);
			}

			if (s_fTimer >= 7)
			{
				s_bStart = true;
				s_fTimer = 0.f;
			}

		}


		if(s_bStart)
		{
			if (nullptr != m_pPlayer)
			{
				m_vTargetPos = m_pPlayer->GetPos();
				Vector2 Area = GetPos();
				if (abs(Area.x - m_vTargetPos.x) < 2200 && abs(Area.y - m_vTargetPos.y) < 2200)
					m_bTarget = true;
				else
					m_bTarget = false;
			}
		}


		if(m_bTarget)
		{
			switch (m_tMonster.HIA)
			{
			case ya::HornetAI::None:
			{
				HornetAI Select = {};
				if (s_iRandom == 2 || s_iRandom == 4 || s_iRandom == 6 || s_iRandom == 7 || s_iRandom == 8)
				{
					if (s_iRandom == 2)
					{
						HornetWalk(400);
					}
					if (s_iRandom == 4)
					{
						HornetWalk(700);
					}
					if (s_iRandom == 6)
					{
						HornetWalk(400);
					}
					if (s_iRandom == 7)
					{
						HornetWalk(300);
					}
					if (s_iRandom == 8)
					{
						HornetWalk(400);
					}
				}
				else
				{
					m_tMonster.HIA = (HornetAI)s_iRandom;
				}



				//m_tMonster.HIA = (HornetAI)s_iRandom;
				//int random = rand() % (9 - 2 + 1) + 2;
				//HornetAI Select = {};
				//m_tMonster.HIA = (HornetAI)random;
			}
			break;
			case ya::HornetAI::Phase:
			{
				
				if (m_Hp <= 25 && m_bPlay == false)
				{
					Hornet_Stun();
				} 

				if (m_Hp <= 17 && m_bChange == false)
				{
					Hornet_Stun();
				}
								
			}
			break;
		 	case ya::HornetAI::Play:
			{
				Hornet_Play();
			}
			break;
			case ya::HornetAI::G_Dash:
			{
				G_Dash_Attack();
			}
			break;
			case ya::HornetAI::A_Dash:
			{
				m_tMonster.HIA = HornetAI::End;
			}
			break;
			case ya::HornetAI::Spear:
			{
				Spear_Attack();
			}
			break;
			case ya::HornetAI::Trap:
			{
				Hornet_Trap();
			}
			break;
			case ya::HornetAI::G_Sphere:
			{
				Sphere_Attack();
			}
			break;
			case ya::HornetAI::A_Sphere:
			{
				Jump_Sphere_Attack();
			}
			break;
			case ya::HornetAI::Defense:
			{				
				Hornet_Defence();
			}
			break;
			case ya::HornetAI::Dumi:
			{
				HornetPushWalk(50);
			}
			break;
			case ya::HornetAI::Ending:
			{
				Hornet_End();
			}
			break;
			case ya::HornetAI::Death:
			{

				this->SetDeathTime(3.f);
			}
			break;
			case ya::HornetAI::End:
			{
				if (m_pPlayer != nullptr)
					m_vTargetPos = m_pPlayer->GetPos();
				m_vMovePos = GetPos();

				if (m_tMonster.HAction == HornetAction::None && s_iCheak <= 2)
				{
					int random = rand() % 3;
					if (!(m_Chaek == (Cheak)random))
						m_Chaek = (Cheak)random;
					s_iCheak++;
				}
				if (s_iCheak > 1)
				{
					m_Chaek = Cheak::None;
					s_iCheak = 0;
				}
				switch (m_Chaek)
				{
				case ya::Cheak::None:
				{
					m_tMonster.HIA = HornetAI::None;
					m_tMonster.HAction = HornetAction::None;

					s_iRandom = rand() % (m_iPhase - 2 + 1) + 2;

					//s_iRandom = rand() % (8 - 2 + 1) + 2;
				}
				break;
				case ya::Cheak::Evade:
				{
					Evade();
				}
				break;
				case ya::Cheak::Jump:
				{
					Hornet_Jump();
				}
				break;
				}

			}
			break;

			}
		}
		
		
		
		GameObject::Tick();
	}

	void Hornet::Render(HDC hdc)
	{


		GameObject::Render(hdc);
	}

	void Hornet::OnCollisionEnter(Collider* other)
	{
		if (Player_Attack* playerObj = dynamic_cast<Player_Attack*>(other->GetOwner()))						
		{
			
			if (m_Hp == 0)
			{
				if (m_Hp == 0 && !(m_bHornet_Death))
				{
					m_tMonster.HIA = HornetAI::Ending;
					m_tMonster.HAction = HornetAction::None;
				}

				return;
			}

			if (m_tMonster.HAction == HornetAction::Play || (m_tMonster.HAction == HornetAction::Trap))
			{
				m_bCounter = true;
				m_pCounter->Play(false);
			}

			else
			{

				if (playerObj->GetSkill() == Player_Skill::Magic || playerObj->GetSkill() == Player_Skill::Magic_Walk)
				{
					playerObj->SetHit(true);
					if (m_Hp > 2)
					{
						m_Hp -= 3;
						m_pHit->Play(false);
					}
					else if (m_Hp < 3)
					{
						m_Hp = 0;
						m_pHit->Play(false);
					}
				}
				else
				{
					if (m_tMonster.HIA == HornetAI::End || m_tMonster.HIA == HornetAI::None)
					{
						--m_Hp;
						m_pHit->Play(false);
						m_tHitSaveAi = m_tMonster;
						if (m_pPlayer != nullptr)
							m_vTargetPos = m_pPlayer->GetPos();
						m_vMovePos = GetPos();
						m_vAttackPos = GetPos();
						m_tMonster.HIA = HornetAI::Dumi;
						m_tMonster.HAction = HornetAction::None;
					}
					else
					{
						--m_Hp;
						m_pHit->Play(false);
					}
				}

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





	}

	void Hornet::OnCollisionStay(Collider* other)
	{
	}

	void Hornet::OnCollisionExit(Collider* other)
	{
	}



	void Hornet::AnimationInitialize()
	{
		//페이즈 변경 및, 첫 등장 
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Play", m_pHornet2Img
			, Vector2(0.0f, 0.0f), Vector2(358.0f, 301.0f)
			, Vector2(50.0f, -30.0f), 14, 0.14f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Play", m_pHornet2Img
			, Vector2(0.0f, 301.0f), Vector2(358.0f, 301.0f)
			, Vector2(-50.0f, -30.0f), 14, 0.14f);


		//G_Dash_Attack(땅돌진) 이미지 3파트 돌진전 + 돌진 + 스탠딩 자세까지 왼쪽, 오른쪽
		m_pHornetAni->CreateAnimation(L"Hornet_Left_G_Dash1", m_pHornet2Img
			, Vector2(0.0f, 602.0f), Vector2(276.0f, 193.0f)
			, Vector2(0.0f, 0.0f), 10, 0.08f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_G_Dash1", m_pHornet2Img
			, Vector2(0.0f, 795.0f), Vector2(276.0f, 193.0f)
			, Vector2(0.0f, 0.0f), 10, 0.08f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_G_Dash2", m_pHornet2Img
			, Vector2(2760.0f, 602.0f), Vector2(255.0f, 138.0f)
			, Vector2(0.0f, 0.0f), 2, 0.08f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_G_Dash2", m_pHornet2Img
			, Vector2(2760.0f, 850.0f), Vector2(255.0f, 138.0f)
			, Vector2(0.0f, 0.0f), 2, 0.08f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_G_Dash3", m_pHornet2Img
			, Vector2(0.0f, 988.0f), Vector2(291.0f, 208.0f)
			, Vector2(0.0f, 0.0f), 6, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_G_Dash3", m_pHornet2Img
			, Vector2(0.0f, 1196.0f), Vector2(291.0f, 208.0f)
			, Vector2(0.0f, 0.0f), 6, 0.1f);


		//A_Dash_Attack(공중돌진) 이미지 3파트 돌진전 + 돌진 + 스탠딩 자세까지 왼쪽, 오른쪽
		m_pHornetAni->CreateAnimation(L"Hornet_Left_A_Dash1", m_pHornet2Img
			, Vector2(0.0f, 1404.0f), Vector2(232.0f, 197.0f)
			, Vector2(0.0f, 0.0f), 9, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_A_Dash1", m_pHornet2Img
			, Vector2(0.0f, 1601.0f), Vector2(232.0f, 197.0f)
			, Vector2(0.0f, 0.0f), 9, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_A_Dash2", m_pHornet2Img
			, Vector2(3270.0f, 602.0f), Vector2(255.0f, 138.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_A_Dash2", m_pHornet2Img
			, Vector2(3270.0f, 850.0f), Vector2(255.0f, 138.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_A_Dash3", m_pHornet2Img
			, Vector2(1746.0f, 988.0f), Vector2(197.0f, 206.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_A_Dash3", m_pHornet2Img
			, Vector2(1746.0f, 1196.0f), Vector2(197.0f, 206.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);


		//Spear_Throw (창 던지기) 이미지 3파스 던지기전 던지기 회수, 왼쪽, 오른쪽
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Spear1", m_pHornet3Img
			, Vector2(0.0f, 0.0f), Vector2(278.0f, 214.0f)
			, Vector2(0.0f, 0.0f), 10, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Spear1", m_pHornet3Img
			, Vector2(0.0f, 214.0f), Vector2(278.0f, 214.0f)
			, Vector2(0.0f, 0.0f), 10, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Spear2", m_pHornet3Img
			, Vector2(0.0f, 428.0f), Vector2(152.0f, 187.0f)
			, Vector2(0.0f, 0.0f), 6, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Spear2", m_pHornet3Img
			, Vector2(0.0f, 615.0f), Vector2(152.0f, 187.0f)
			, Vector2(0.0f, 0.0f), 6, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Spear3", m_pHornet3Img
			, Vector2(0.0f, 802.0f), Vector2(240.0f, 189.0f)
			, Vector2(0.0f, 0.0f), 6, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Spear3", m_pHornet3Img
			, Vector2(0.0f, 991.0f), Vector2(240.0f, 189.0f)
			, Vector2(0.0f, 0.0f), 6, 0.1f);



		//Counter_Attack (검으로 막고 카운터 공격) 실패 1 or 성공 4  공통 시작 2 총 7개  왼쪽, 오른쪽
		//Counter Start + Hold
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Counter_Start", m_pHornet3Img
			, Vector2(912.0f, 428.0f), Vector2(185.0f, 215.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Counter_Start", m_pHornet3Img
			, Vector2(1282.0f, 428.0f), Vector2(185.0f, 215.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Counter_Hold", m_pHornet3Img
			, Vector2(1440.0f, 802.0f), Vector2(185.0f, 144.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Counter_Hold", m_pHornet3Img
			, Vector2(1440.0f, 946.0f), Vector2(185.0f, 144.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);

		//실패
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Counter_Fail", m_pHornet3Img
			, Vector2(2022.0f, 428.0f), Vector2(112.0f, 213.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Counter_Fail", m_pHornet3Img
			, Vector2(2246.0f, 428.0f), Vector2(112.0f, 213.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);

		//성공
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Counter_Attack1", m_pHornet3Img
			, Vector2(0.0f, 1180.0f), Vector2(260.0f, 280.0f)
			, Vector2(0.0f, -50.0f), 3, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Counter_Attack1", m_pHornet3Img
			, Vector2(0.0f, 1460.0f), Vector2(260.0f, 280.0f)
			, Vector2(0.0f, -50.0f), 3, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Counter_Attack2", m_pHornet3Img
			, Vector2(780.0f, 1180.0f), Vector2(360.0f, 372.0f)
			, Vector2(-100.0f, -80.0f), 1, 0.12f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Counter_Attack3", m_pHornet3Img
			, Vector2(1140.0f, 1180.0f), Vector2(598.0f, 333.0f)
			, Vector2(-70.0f, -80.0f), 1, 0.12f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Counter_Attack2", m_pHornet3Img
			, Vector2(2181.0f, 641.0f), Vector2(360.0f, 372.0f)
			, Vector2(100.0f, -80.0f), 1, 0.12f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Counter_Attack3", m_pHornet3Img
			, Vector2(1738.0f, 1379.0f), Vector2(598.0f, 333.0f)
			, Vector2(70.0f, -80.0f), 1, 0.12f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Counter_Attack4", m_pHornet3Img
			, Vector2(1738.0f, 1180.0f), Vector2(237.0f, 199.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Counter_Attack4", m_pHornet3Img
			, Vector2(2212.0f, 1180.0f), Vector2(237.0f, 199.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);

		//Trap 원거리 가시 설치하기 이미지 3파트 왼쪽, 오른쪽
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Trap1", m_pHornet3Img
			, Vector2(0.0f, 1740.0f), Vector2(168.0f, 226.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Trap1", m_pHornet3Img
			, Vector2(0.0f, 1966.0f), Vector2(168.0f, 226.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Trap2", m_pHornet3Img
			, Vector2(1837.0f, 1740.0f), Vector2(137.0f, 189.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Trap2", m_pHornet3Img
			, Vector2(1837.0f, 1976.0f), Vector2(137.0f, 189.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Trap3", m_pHornet3Img
			, Vector2(672.0f, 1740.0f), Vector2(233.0f, 216.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Trap3", m_pHornet3Img
			, Vector2(672.0f, 1976.0f), Vector2(233.0f, 216.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);



		//Sphere_Attack(구체기) 이미지 4파트 공중 or 땅 시작 각각 하나씩 구체공격, 스탠딩 해서 4개 왼쪽, 오른쪽
		m_pHornetAni->CreateAnimation(L"Hornet_Left_G_Sphere1", m_pHornet2Img
			, Vector2(0.0f, 1798.0f), Vector2(236.0f, 201.0f)
			, Vector2(0.0f, 0.0f), 10, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_G_Sphere1", m_pHornet2Img
			, Vector2(0.0f, 1999.0f), Vector2(236.0f, 201.0f)
			, Vector2(0.0f, 0.0f), 10, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_A_Sphere1", m_pHornet2Img
			, Vector2(0.0f, 2200.0f), Vector2(241.0f, 204.0f)
			, Vector2(0.0f, 0.0f), 7, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_A_Sphere1", m_pHornet2Img
			, Vector2(0.0f, 2404.0f), Vector2(241.0f, 204.0f)
			, Vector2(0.0f, 0.0f), 7, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Sphere2", m_pHornet2Img
			, Vector2(2360.0f, 1798.0f), Vector2(141.0f, 184.0f)
			, Vector2(0.0f, 0.0f), 9, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Sphere2", m_pHornet2Img
			, Vector2(2360.0f, 2001.0f), Vector2(141.0f, 184.0f)
			, Vector2(0.0f, 0.0f), 9, 0.1f);

		m_pHornetAni->CreateAnimation(L"Hornet_Left_Sphere_End", m_pHornet2Img
			, Vector2(3780.0f, 602.0f), Vector2(273.0f, 210.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Sphere_End", m_pHornet2Img
			, Vector2(3780.0f, 850.0f), Vector2(273.0f, 210.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);




		//Standing 이미지 왼쪽, 오른쪽
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Standing", m_pHornet1Img
			, Vector2(0.0f, 0.0f), Vector2(184.0f, 216.0f)
			, Vector2(0.0f, 0.0f), 6, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Standing", m_pHornet1Img
			, Vector2(0.0f, 216.0f), Vector2(184.0f, 216.0f)
			, Vector2(0.0f, 0.0f), 6, 0.1f);
		//Walk 이미지 왼쪽, 오른쪽
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Walk", m_pHornet1Img
			, Vector2(0.0f, 432.0f), Vector2(159.0f, 191.0f)
			, Vector2(0.0f, 0.0f), 8, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Walk", m_pHornet1Img
			, Vector2(0.0f, 623.0f), Vector2(159.0f, 191.0f)
			, Vector2(0.0f, 0.0f), 8, 0.1f);
		//Jump 이미지 (착지까지 3개) + 공중공격으로 착지 하는것 1개 해서 4개
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Jump1", m_pHornet1Img
			, Vector2(0.0f, 814.0f), Vector2(207.0f, 199.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Jump1", m_pHornet1Img
			, Vector2(0.0f, 1013.0f), Vector2(207.0f, 199.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Jump2", m_pHornet1Img
			, Vector2(0.0f, 1212.0f), Vector2(188.0f, 228.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Jump2", m_pHornet1Img
			, Vector2(0.0f, 1440.0f), Vector2(188.0f, 228.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Jump3", m_pHornet1Img
			, Vector2(940.0f, 1212.0f), Vector2(188.0f, 228.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Jump3", m_pHornet1Img
			, Vector2(940.0f, 1440.0f), Vector2(188.0f, 228.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Land_Default", m_pHornet1Img
			, Vector2(0.0f, 1668.0f), Vector2(203.0f, 218.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Land_Default", m_pHornet1Img
			, Vector2(0.0f, 1886.0f), Vector2(203.0f, 218.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Land_Attack", m_pHornet1Img
			, Vector2(0.0f, 2104.0f), Vector2(207.0f, 230.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Land_Attack", m_pHornet1Img
			, Vector2(0.0f, 2334.0f), Vector2(207.0f, 230.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);

		//Back_Step (회피기 뒤로 물러서기) (이미지 착지전까지 2개) 왼쪽, 오른쪽  
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Evade1", m_pHornet1Img
			, Vector2(0.0f, 2564.0f), Vector2(185.0f, 217.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Evade1", m_pHornet1Img
			, Vector2(0.0f, 2781.0f), Vector2(185.0f, 217.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Evade2", m_pHornet1Img
			, Vector2(555.0f, 2564.0f), Vector2(196.0f, 212.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Evade2", m_pHornet1Img
			, Vector2(555.0f, 2781.0f), Vector2(196.0f, 212.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);


		//벽에 붙어있는 이미지 왼쪽, 오른쪽
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Wall", m_pHornet1Img
			, Vector2(609.0f, 1668.0f), Vector2(127.0f, 213.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Wall", m_pHornet1Img
			, Vector2(609.0f, 1886.0f), Vector2(127.0f, 213.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);


		//스턴 상태 이미지 3피스 왼쪽, 오른쪽
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Stun1", m_pHornet3Img
			, Vector2(0.0f, 2192.0f), Vector2(300.0f, 227.0f)
			, Vector2(0.0f, 0.0f), 6, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Stun1", m_pHornet3Img
			, Vector2(0.0f, 2419.0f), Vector2(300.0f, 227.0f)
			, Vector2(0.0f, 0.0f), 6, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Stun2", m_pHornet3Img
			, Vector2(0.0f, 2646.0f), Vector2(212.0f, 187.0f)
			, Vector2(0.0f, 0.0f), 2, 0.2f);
		m_pHornetAni->CreateAnimation(L"Hornet_Left_Stun3", m_pHornet3Img
			, Vector2(424.0f, 2646.0f), Vector2(212.0f, 187.0f)
			, Vector2(0.0f, 0.0f), 4, 0.2f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Stun2", m_pHornet3Img
			, Vector2(0.0f, 2833.0f), Vector2(212.0f, 187.0f)
			, Vector2(0.0f, 0.0f), 2, 0.2f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Stun3", m_pHornet3Img
			, Vector2(424.0f, 2833.0f), Vector2(212.0f, 187.0f)
			, Vector2(0.0f, 0.0f), 4, 0.2f);

		m_pHornetAni->CreateAnimation(L"Hornet_Left_Stun4", m_pHornet3Img
			, Vector2(0.0f, 3020.0f), Vector2(212.0f, 216.0f)
			, Vector2(0.0f, 0.0f), 7, 0.1f);
		m_pHornetAni->CreateAnimation(L"Hornet_Right_Stun4", m_pHornet3Img
			, Vector2(0.0f, 3236.0f), Vector2(212.0f, 216.0f)
			, Vector2(0.0f, 0.0f), 7, 0.1f);

		//Effect 모음
		m_pHornetAniEffect->CreateAnimation(L"Hornet_Left_G_Dash_Effect", m_pHornetEffectImg
			, Vector2(0.0f, 0.0f), Vector2(833.0f, 370.0f)
			, Vector2(480.0f, -60.0f), 7, 0.05f);
		m_pHornetAniEffect->CreateAnimation(L"Hornet_Right_G_Dash_Effect", m_pHornetEffectImg
			, Vector2(0.0f, 370.0f), Vector2(833.0f, 370.0f)
			, Vector2(-480.0f, -60.0f), 7, 0.05f);
		m_pHornetAniEffect->CreateAnimation(L"Hornet_Left_A_Dash_Effect", m_pHornetEffectImg
			, Vector2(0.0f, 740.0f), Vector2(451.0f, 568.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		m_pHornetAniEffect->CreateAnimation(L"Hornet_Right_A_Dash_Effect", m_pHornetEffectImg
			, Vector2(0.0f, 1308.0f), Vector2(451.0f, 568.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		m_pHornetAniEffect->CreateAnimation(L"Hornet_Left_Sphere_Effect", m_pHornetEffectImg
			, Vector2(0.0f, 1876.0f), Vector2(427.0f, 447.0f)
			, Vector2(0.0f, 0.0f), 10, 0.1f);
		m_pHornetAniEffect->CreateAnimation(L"Hornet_Right_Sphere_Effect", m_pHornetEffectImg
			, Vector2(0.0f, 2323.0f), Vector2(427.0f, 447.0f)
			, Vector2(0.0f, 0.0f), 10, 0.1f);
		m_pHornetAniEffect->CreateAnimation(L"Hornet_Right_Spear_Effect", m_pHornetEffectImg
			, Vector2(3816.0f, 1300.0f), Vector2(368.0f, 175.0f)
			, Vector2(-100.0f, 30.0f), 4, 0.05f);
		m_pHornetAniEffect->CreateAnimation(L"Hornet_Left_Spear_Effect", m_pHornetEffectImg
			, Vector2(3816.0f, 1475.0f), Vector2(368.0f, 175.0f)
			, Vector2(100.0f, 30.0f), 4, 0.05f);



		//마지막에 죽을때 터지는 이펙트 한방향만 있음
		m_pHornetAniEffect->CreateAnimation(L"Hornet_Last_Effect", m_pHornetEndImg
			, Vector2(0.0f, 0.0f), Vector2(1843.0f, 614.0f)
			, Vector2(0.0f, 0.0f), 5, 0.3f);


	}

	void Hornet::SoundInitialize()
	{

		if (nullptr == m_pStart)
			m_pStart = Resources::Load<Sound>(L"Hornet_Start_S", L"..\\Resources\\Sound\\Hornet_Start_S.wav");
		if (nullptr == m_pLeftEvade)
			m_pLeftEvade = Resources::Load<Sound>(L"Hornet_Left_Laugh_S", L"..\\Resources\\Sound\\Hornet_Left_Laugh_S.wav");
		if (nullptr == m_pRightEvade)
			m_pRightEvade = Resources::Load<Sound>(L"Hornet_Right_Laugh_S", L"..\\Resources\\Sound\\Hornet_Right_Laugh_S.wav");
		if (nullptr == m_pWalk)
			m_pWalk = Resources::Load<Sound>(L"hornet_Walk_S", L"..\\Resources\\Sound\\hornet_Walk_S.wav");
		if (nullptr == m_pG_Dash)
			m_pG_Dash = Resources::Load<Sound>(L"Hornet_G_Dash_S", L"..\\Resources\\Sound\\Hornet_G_Dash_S.wav");
		if (nullptr == m_pDash)
			m_pDash = Resources::Load<Sound>(L"hornet_dash_S", L"..\\Resources\\Sound\\hornet_dash_S.wav");
		if (nullptr == m_pSphere)
			m_pSphere = Resources::Load<Sound>(L"Hornet_Sphere_S", L"..\\Resources\\Sound\\Hornet_Sphere_S.wav");
		if (nullptr == m_pSphere1)
			m_pSphere1 = Resources::Load<Sound>(L"Hornet_Sphere_S1", L"..\\Resources\\Sound\\Hornet_Sphere_S1.wav");
		if (nullptr == m_pA_Sphere)
			m_pA_Sphere = Resources::Load<Sound>(L"Hornet_A_Sphere_S", L"..\\Resources\\Sound\\Hornet_A_Sphere_S.wav");
		if (nullptr == m_pJump)
			m_pJump = Resources::Load<Sound>(L"Hornet_Jump_S", L"..\\Resources\\Sound\\Hornet_Jump_S.wav");
		if (nullptr == m_pS_Jump)
			m_pS_Jump = Resources::Load<Sound>(L"hornet_Small_Jump_S", L"..\\Resources\\Sound\\hornet_Small_Jump_S.wav");
		if (nullptr == m_pJump_Effect)
			m_pJump_Effect = Resources::Load<Sound>(L"hornet_jump_Effect_S", L"..\\Resources\\Sound\\hornet_jump_Effect_S.wav");
		if (nullptr == m_pLand)
			m_pLand = Resources::Load<Sound>(L"hornet_Land_S", L"..\\Resources\\Sound\\hornet_Land_S.wav");
		if (nullptr == m_pSpear)
			m_pSpear = Resources::Load<Sound>(L"Hornet_Spear_S", L"..\\Resources\\Sound\\Hornet_Spear_S.wav");
		if (nullptr == m_pSpear1)
			m_pSpear1 = Resources::Load<Sound>(L"hornet_Spear_S1", L"..\\Resources\\Sound\\hornet_Spear_S1.wav");
		if (nullptr == m_pSpear2)
			m_pSpear2 = Resources::Load<Sound>(L"Hornet_Spear_S2", L"..\\Resources\\Sound\\hornet_Spear_S2.wav");
		if (nullptr == m_pTrap)
			m_pTrap = Resources::Load<Sound>(L"hornet_Trap_S", L"..\\Resources\\Sound\\hornet_Trap_S.wav");
		if (nullptr == m_pTrap1)
			m_pTrap1 = Resources::Load<Sound>(L"hornet_Trap_S1", L"..\\Resources\\Sound\\hornet_Trap_S1.wav");
		if (nullptr == m_pCounter)
			m_pCounter = Resources::Load<Sound>(L"hornet_Counter_S", L"..\\Resources\\Sound\\hornet_Counter_S.wav");
		if (nullptr == m_pCounter1)
			m_pCounter1 = Resources::Load<Sound>(L"hornet_Counter_S1", L"..\\Resources\\Sound\\hornet_Counter_S1.wav");
		if (nullptr == m_pStun)
			m_pStun = Resources::Load<Sound>(L"Hornet_Stun_S", L"..\\Resources\\Sound\\Hornet_Stun_S.wav");
		if (nullptr == m_pDeath)
			m_pDeath = Resources::Load<Sound>(L"Hornet_Death_S", L"..\\Resources\\Sound\\Hornet_Death_S.wav");
		if (nullptr == m_pHit)
			m_pHit = Resources::Load<Sound>(L"Monster_Damage_S", L"..\\Resources\\Sound\\Monster_Damage_S.wav");

		

	}


	void Hornet::G_Dash_Attack()
	{
		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();

		if (m_tMonster.HAction == HornetAction::None)
		{
			//플에이어 위치가 오른쪽에 있다
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Right_G_Dash1", false);
				m_tMonster.Direction = DIRECTION::Right;
				m_tMonster.HAction = HornetAction::G_Dash;
			}
			//플에이어 위치가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Left_G_Dash1", false);
				m_tMonster.Direction = DIRECTION::Left;
				m_tMonster.HAction = HornetAction::G_Dash;
			}	
			
		}
		if (m_tMonster.HAction == HornetAction::G_Dash)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.8f)
			{
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pHornetAni->Play(L"Hornet_Right_G_Dash2", true);
					m_pHornetAniEffect->Play(L"Hornet_Right_G_Dash_Effect", false);
					m_pMonsterSize->SetScale(Vector2(250.f, 145.f));
					m_pMonsterSize->SetOffset(Vector2(0.f, 5.f));
				}
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pHornetAni->Play(L"Hornet_Left_G_Dash2", true);
					m_pHornetAniEffect->Play(L"Hornet_Left_G_Dash_Effect", false);
					m_pMonsterSize->SetScale(Vector2(250.f, 145.f));
					m_pMonsterSize->SetOffset(Vector2(0.f, 5.f));
				}

				m_pG_Dash->Play(false);
				m_pDash->Play(false);
				m_tMonster.HAction = HornetAction::Before_End;
				m_vAttackPos = GetPos();
				s_fTimer = 0.f;

			}
		}

		if (m_tMonster.HAction == HornetAction::Before_End)
		{

			if (m_tMonster.Direction == DIRECTION::Right)
			{
				//if (m_vAttackPos.x + 600 >= m_vMovePos.x)
				if (m_vAttackPos.x + 600 >= m_vMovePos.x && (!m_bRightWalkCheak))
					m_vMovePos.x += 1200.f * DT;
				else
				{
					m_pMonsterSize->SetScale(Vector2(180.0f, 220.0f));
					m_pMonsterSize->SetOffset(Vector2(0.f, 0.f));
					m_pHornetAni->Play(L"Hornet_Right_G_Dash3", false);
					m_tMonster.HAction = HornetAction::End;				
				}
			}

			if (m_tMonster.Direction == DIRECTION::Left)
			{
				//if (m_vAttackPos.x - 600 <= m_vMovePos.x)
				if (m_vAttackPos.x - 600 <= m_vMovePos.x && (!m_bLeftWalkCheak))
					m_vMovePos.x -= 1200.f * DT;
				else
				{
					m_pMonsterSize->SetScale(Vector2(180.0f, 220.0f));
					m_pMonsterSize->SetOffset(Vector2(0.f, 0.f));
					m_pHornetAni->Play(L"Hornet_Left_G_Dash3", false);
					m_tMonster.HAction = HornetAction::End;
				}
			}
			SetPos(m_vMovePos);
		}

		if (m_tMonster.HAction == HornetAction::End)
		{
			s_fTimer += DT;
			if (s_fTimer > 0.6f)
			{
				if (m_vTargetPos.x > m_vMovePos.x)				
					m_pHornetAni->Play(L"Hornet_Right_Standing", true);
				if (m_vTargetPos.x < m_vMovePos.x)
					m_pHornetAni->Play(L"Hornet_Left_Standing", true);
				m_tMonster.HAction = HornetAction::None;
				m_tMonster.HIA = HornetAI::End;
				s_fTimer = 0.f;
			}
		}

	}

	void Hornet::Sphere_Attack()
	{
		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();

		if (m_tMonster.HAction == HornetAction::None)
		{
			m_pSphere->Play(false);
			//플에이어 위치가 오른쪽에 있다
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Right_G_Sphere1", false);
				m_tMonster.Direction = DIRECTION::Right;
				m_tMonster.HAction = HornetAction::G_Sphere;
			}
			//플에이어 위치가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Left_G_Sphere1", false);
				m_tMonster.Direction = DIRECTION::Left;
				m_tMonster.HAction = HornetAction::G_Sphere;
			}
		}
		if (m_tMonster.HAction == HornetAction::G_Sphere)
		{
			s_fTimer += DT;
			if (s_fTimer >= 1.0f)
			{
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pHornetAni->Play(L"Hornet_Right_Sphere2", true);
					m_pHornetAniEffect->Play(L"Hornet_Right_Sphere_Effect", false);		
				}
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pHornetAni->Play(L"Hornet_Left_Sphere2", true);
					m_pHornetAniEffect->Play(L"Hornet_Left_Sphere_Effect", false);	
				}
				m_pSphere1->Play(false);
				m_pMonsterSize->SetScale(Vector2(430.f, 450.f));
				m_pMonsterSize->SetOffset(Vector2(0.f, 0.f));
				m_tMonster.HAction = HornetAction::Before_End;
				s_fTimer = 0.f;
			}
		}

		if (m_tMonster.HAction == HornetAction::Before_End)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.9f)
			{
				if (m_tMonster.Direction == DIRECTION::Right)								
					m_pHornetAni->Play(L"Hornet_Right_Sphere_End", false);
				if (m_tMonster.Direction == DIRECTION::Left)
					m_pHornetAni->Play(L"Hornet_Left_Sphere_End", false);

				m_pMonsterSize->SetScale(Vector2(180.0f, 220.0f));
				m_tMonster.HAction = HornetAction::End;
				s_fTimer = 0.f;
			}
		}

		if (m_tMonster.HAction == HornetAction::End)
		{
			s_fTimer += DT;
			if (s_fTimer > 0.2f)
			{
				if (m_vTargetPos.x > m_vMovePos.x)
					m_pHornetAni->Play(L"Hornet_Right_Standing", true);
				if (m_vTargetPos.x < m_vMovePos.x)
					m_pHornetAni->Play(L"Hornet_Left_Standing", true);

				m_tMonster.HAction = HornetAction::None;
				m_tMonster.HIA = HornetAI::End;
				s_fTimer = 0.f;
			}
		}
	}

	void Hornet::Spear_Attack()
	{
		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();

		if (m_tMonster.HAction == HornetAction::None)
		{
			
			//플에이어 위치가 오른쪽에 있다
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				// 창 던지는 모션
				m_pHornetAni->Play(L"Hornet_Right_Spear1", false);
				m_tMonster.Direction = DIRECTION::Right;
				m_tMonster.HAction = HornetAction::Spear;
			}
			//플에이어 위치가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				// 창 던지는 모션
				m_pHornetAni->Play(L"Hornet_Left_Spear1", false);
				m_tMonster.Direction = DIRECTION::Left;
				m_tMonster.HAction = HornetAction::Spear;
			}
		}
		if (m_tMonster.HAction == HornetAction::Spear)
		{
			s_fTimer += DT;
			if (s_fTimer >= 1.0f)
			{
				m_pSpear->Play(false);
				m_pSpear1->Play(false);
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					//창 던지고 이펙트 켜주고 다음
					m_pHornetAni->Play(L"Hornet_Right_Spear2", false);
					m_pHornetAniEffect->Play(L"Hornet_Right_Spear_Effect", false);
				}
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					//창 던지고 이펙트 켜주고 다음
					m_pHornetAni->Play(L"Hornet_Left_Spear2", false);
					m_pHornetAniEffect->Play(L"Hornet_Left_Spear_Effect", false);
				}

				m_pMonsterSize->SetScale(Vector2(200.f, 135.f));
				m_pMonsterSize->SetOffset(Vector2(0.f, 30.f));
				m_Attack = ya::object::Instantiate<Hornet_Attack>(eColliderLayer::Monster_Projecttile);				
				m_Attack->SetPos(GetPos());
				m_tMonster.HAction = HornetAction::Walk;
				m_vAttackPos = GetPos();
				s_fTimer = 0.f;

			}
		}

		if (m_tMonster.HAction == HornetAction::Walk)
		{
			if(nullptr != m_Attack)
			m_vMovePos = m_Attack->GetPos();
			
			if (m_tMonster.Direction == DIRECTION::Right)
			{
				//지정 해둔 방향까지 창 이동 , 지정해둔 방향 도착하면 0.3초 뒤에 다음
				m_Attack->SetAttack_Type(Attack_Type::Right_Spear);
				if (m_vAttackPos.x + 900 >= m_vMovePos.x)
					m_vMovePos.x += 1700.f * DT;
				else
				{		
					s_fTimer += DT;
					if (s_fTimer >= 0.3f)
					{
						Hornet_Attack* Effect = ya::object::Instantiate<Hornet_Attack>(eColliderLayer::Monster_Projecttile);
						Effect->SetPos(m_vMovePos);
						Effect->SetAttack_Type(Attack_Type::Right_Spear_Effect);
						m_tMonster.HAction = HornetAction::Before_End;
						s_fTimer = 0.f;
					}
				}
			}

			if (m_tMonster.Direction == DIRECTION::Left)
			{
				//지정 해둔 방향까지 창 이동 , 지정해둔 방향 도착하면 0.3초 뒤에 다음
				m_Attack->SetAttack_Type(Attack_Type::Left_Spear);
				if (m_vAttackPos.x - 900 <= m_vMovePos.x)
					m_vMovePos.x -= 1700.f * DT;
				else
				{			
					s_fTimer += DT;
					if (s_fTimer >= 0.3f)
					{
						Hornet_Attack* Effect = ya::object::Instantiate<Hornet_Attack>(eColliderLayer::Monster_Projecttile);
						Effect->SetPos(m_vMovePos);
						Effect->SetAttack_Type(Attack_Type::Left_Spear_Effect);
						m_tMonster.HAction = HornetAction::Before_End;
						s_fTimer = 0.f;
					}
				}
			}
			m_Attack->SetPos(m_vMovePos);
		}

		if (m_tMonster.HAction == HornetAction::Before_End)
		{
			m_vMovePos = m_Attack->GetPos();

			if (m_tMonster.Direction == DIRECTION::Right)
			{
				
				if (m_vAttackPos.x <= m_vMovePos.x)
					m_vMovePos.x -= 1700.f * DT;
				else
				{
					m_pSpear2->Play(false);
					m_Attack->Death();
					m_pHornetAni->Play(L"Hornet_Right_Spear3", false);
					m_tMonster.HAction = HornetAction::End;		

				}
			}

			if (m_tMonster.Direction == DIRECTION::Left)
			{
				
				if (m_vAttackPos.x >= m_vMovePos.x)
					m_vMovePos.x += 1700.f * DT;
				else
				{
					m_pSpear2->Play(false);
					m_Attack->Death();
					m_pHornetAni->Play(L"Hornet_Left_Spear3", false);
					m_tMonster.HAction = HornetAction::End;
				}
			}
			m_Attack->SetPos(m_vMovePos);
		}

		if (m_tMonster.HAction == HornetAction::End)
		{
			s_fTimer += DT;
			if (s_fTimer > 0.6f)
			{
				if (m_vTargetPos.x > m_vMovePos.x)
					m_pHornetAni->Play(L"Hornet_Right_Standing", true);
				if (m_vTargetPos.x < m_vMovePos.x)
					m_pHornetAni->Play(L"Hornet_Left_Standing", true);

				m_pMonsterSize->SetScale(Vector2(180.f, 220.f));
				m_pMonsterSize->SetOffset(Vector2(0.f, 0.f));
				m_tMonster.HAction = HornetAction::None;
				m_tMonster.HIA = HornetAI::End;
				s_fTimer = 0.f;
			}
		}

	}

	void Hornet::Hornet_Jump()
	{
		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();

		
		if (m_tMonster.HAction == HornetAction::None)
		{
			//플에이어 위치가 오른쪽에 있다
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Right_Jump1", false);
				m_tMonster.Direction = DIRECTION::Right;
				m_tMonster.HAction = HornetAction::Jump;
			}
			//플에이어 위치가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Left_Jump1", false);
				m_tMonster.Direction = DIRECTION::Left;
				m_tMonster.HAction = HornetAction::Jump;
			}
		}

		if (m_tMonster.HAction == HornetAction::Jump)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.4f)
			{
				//(최대 - 최소 + 1) + 최소
				//왼쪽으로 300 제한 위로 700 제한 (랜덤하게)
				int Col = rand() % (300 - 100 + 1) + 100;
				int Rol = rand() % (700 - 550 + 1) + 550;

				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pHornetAni->Play(L"Hornet_Right_Jump2", false);
					Col *= 1;
					
				}
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pHornetAni->Play(L"Hornet_Left_Jump2", false);
					Col *= -1;
				}

				Rol *= -1;
				m_vAttackPos = GetPos();
				m_vAttackPos += Vector2((float)Col, (float)Rol);
				m_tMonster.HAction = HornetAction::Walk;
				m_pS_Jump->Play(false);
				m_pJump_Effect->Play(false);
				s_fTimer = 0.f;
			}
		}

		if (m_tMonster.HAction == HornetAction::Walk)
		{
			if (m_tMonster.Direction == DIRECTION::Right)
			{
				if (m_vAttackPos.x >= m_vMovePos.x && m_vAttackPos.y <= m_vMovePos.y && !(m_bRightWalkCheak))
				{		
					Vector2 POS = m_vAttackPos - m_vMovePos;
					POS.Normalize();					
					m_vMovePos.x += POS.x * 1200.f * DT;
					m_vMovePos.y += POS.y * 1200.f * DT;
				}
				else
				{
					Rigidbody * rigidbody = GetComponent<Rigidbody>();
					if (!(rigidbody->GetGround()))
					rigidbody->SetGround(false);
					m_tMonster.HAction = HornetAction::Before_End;
					m_pHornetAni->Play(L"Hornet_Right_Jump3", true);					
				}
			}

			if (m_tMonster.Direction == DIRECTION::Left)
			{
				if (m_vAttackPos.x <= m_vMovePos.x && m_vAttackPos.y <= m_vMovePos.y && !(m_bLeftWalkCheak))
				{
					Vector2 POS = m_vAttackPos - m_vMovePos;
					POS.Normalize();					
					m_vMovePos.x += POS.x * 1200.f * DT;
					m_vMovePos.y += POS.y * 1200.f * DT;
				}
				else
				{
					Rigidbody* rigidbody = GetComponent<Rigidbody>();
					if (!(rigidbody->GetGround()))
					rigidbody->SetGround(false);
					m_tMonster.HAction = HornetAction::Before_End;
					m_pHornetAni->Play(L"Hornet_Left_Jump3", true);							
				}
			}
			SetPos(m_vMovePos);	

		}

		if (m_tMonster.HAction == HornetAction::Before_End)
		{
			if (GetComponent<Rigidbody>()->GetGround())
			{
				if (m_tMonster.Direction == DIRECTION::Right)
					m_pHornetAni->Play(L"Hornet_Right_Land_Default", false);
				if (m_tMonster.Direction == DIRECTION::Left)
					m_pHornetAni->Play(L"Hornet_Left_Land_Default", false);

				m_pLand->Play(false);
				m_tMonster.HAction = HornetAction::End;			
			}
		}

		if (m_tMonster.HAction == HornetAction::End)
		{
			s_fTimer += DT;
			if(s_fTimer > 0.3f)
			{				
				if (m_vTargetPos.x > m_vMovePos.x)
					m_pHornetAni->Play(L"Hornet_Right_Standing", true);
				if (m_vTargetPos.x < m_vMovePos.x)
					m_pHornetAni->Play(L"Hornet_Left_Standing", true);

				m_tMonster.HAction = HornetAction::None;
				m_tMonster.HIA = HornetAI::End;
				s_fTimer = 0.f;
			}
		}

	}


	
	void Hornet::Jump_Sphere_Attack()
	{
		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();


		if (m_tMonster.HAction == HornetAction::None)
		{			
			//플에이어 위치가 오른쪽에 있다
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Right_Jump1", false);
				m_tMonster.Direction = DIRECTION::Right;
				m_tMonster.HAction = HornetAction::Jump;
			}
			
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Left_Jump1", false);
				m_tMonster.Direction = DIRECTION::Left;
				m_tMonster.HAction = HornetAction::Jump;
			}
			
		}

		if (m_tMonster.HAction == HornetAction::Jump)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.4f)
			{
				//(최대 - 최소 + 1) + 최소
				//왼쪽으로 300 제한 위로 700 제한 (랜덤하게)
				int Col = rand() % (300 - 100 + 1) + 100;
				int Rol = rand() % (600 - 200 + 1) + 200;

				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pHornetAni->Play(L"Hornet_Right_Jump2", false);
					Col *= 1;

				}
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pHornetAni->Play(L"Hornet_Left_Jump2", false);
					Col *= -1;
				}

				Rol *= -1;
				m_vAttackPos = GetPos();
				m_vAttackPos += Vector2((float)Col, (float)Rol);
				m_tMonster.HAction = HornetAction::Walk;
				m_pJump->Play(false);
				m_pJump_Effect->Play(false);
				s_fTimer = 0.f;
			}
		}

		if (m_tMonster.HAction == HornetAction::Walk)
		{
			if (m_tMonster.Direction == DIRECTION::Right)
			{
				if (m_vAttackPos.x >= m_vMovePos.x && m_vAttackPos.y <= m_vMovePos.y && !(m_bRightWalkCheak))
				{
					Vector2 POS = m_vAttackPos - m_vMovePos;
					POS.Normalize();					
					m_vMovePos.x += POS.x * 1200.f * DT;
					m_vMovePos.y += POS.y * 1200.f * DT;
				}
				else
				{
					m_pA_Sphere->Play(false);
					m_pHornetAni->Play(L"Hornet_Right_A_Sphere1", false);
					m_tMonster.Direction = DIRECTION::Right;
					//if (m_vTargetPos.x > m_vMovePos.x)
					//{
					//	m_pHornetAni->Play(L"Hornet_Right_A_Sphere1", false);
					//	m_tMonster.Direction = DIRECTION::Right;
					//}
					//if (m_vTargetPos.x < m_vMovePos.x)
					//{
					//	m_pHornetAni->Play(L"Hornet_Left_A_Sphere1", false);
					//	m_tMonster.Direction = DIRECTION::Left;
					//}
					Rigidbody* rigidbody = GetComponent<Rigidbody>();
					if (!(rigidbody->GetGround()))
					rigidbody->SetGround(true);
					m_tMonster.HAction = HornetAction::A_Sphere;	
				}
			}

			if (m_tMonster.Direction == DIRECTION::Left)
			{
				if (m_vAttackPos.x <= m_vMovePos.x && m_vAttackPos.y <= m_vMovePos.y && !(m_bLeftWalkCheak))
				{
					Vector2 POS = m_vAttackPos - m_vMovePos;
					POS.Normalize();					
					m_vMovePos.x += POS.x * 1200.f * DT;
					m_vMovePos.y += POS.y * 1200.f * DT;
				}
				else
				{
					m_pA_Sphere->Play(false);					
					m_pHornetAni->Play(L"Hornet_Left_A_Sphere1", false);
					m_tMonster.Direction = DIRECTION::Left;
					//if (m_vTargetPos.x > m_vMovePos.x)
					//{
					//	m_pHornetAni->Play(L"Hornet_Right_A_Sphere1", false);
					//	m_tMonster.Direction = DIRECTION::Right;
					//}
					//if (m_vTargetPos.x < m_vMovePos.x)
					//{
					//	m_pHornetAni->Play(L"Hornet_Left_A_Sphere1", false);
					//	m_tMonster.Direction = DIRECTION::Left;
					//}
					Rigidbody* rigidbody = GetComponent<Rigidbody>();
					if (!(rigidbody->GetGround()))
					rigidbody->SetGround(true);
					m_tMonster.HAction = HornetAction::A_Sphere;
				}
			}
			SetPos(m_vMovePos);

		}
		if (m_tMonster.HAction == HornetAction::A_Sphere)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.7f)
			{
				if (m_vTargetPos.x > m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Right_Sphere2", false);
					m_pHornetAniEffect->Play(L"Hornet_Right_Sphere_Effect", false);
					m_tMonster.Direction = DIRECTION::Right;
				}

				if (m_vTargetPos.x < m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Left_Sphere2", false);
					m_pHornetAniEffect->Play(L"Hornet_Left_Sphere_Effect", false);
					m_tMonster.Direction = DIRECTION::Left;
				}			
				m_pSphere1->Play(false);
				m_pMonsterSize->SetScale(Vector2(430.f, 450.f));
				m_pMonsterSize->SetOffset(Vector2(0.f, 0.f));
				m_tMonster.HAction = HornetAction::Drop;
				s_fTimer = 0.f;
			}
		}

		if (m_tMonster.HAction == HornetAction::Drop)
		{			
			s_fTimer += DT;
			if (s_fTimer >= 0.9f)
			{
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pHornetAni->Play(L"Hornet_Left_Jump3", true);					
				}
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pHornetAni->Play(L"Hornet_Right_Jump3", true);
				}
				m_pMonsterSize->SetScale(Vector2(180.0f, 220.0f));
				Rigidbody* rigidbody = GetComponent<Rigidbody>();
				rigidbody->SetGround(false);
				m_tMonster.HAction = HornetAction::Before_End;	
				s_fTimer = 0.f;
			}	
		}
		if (m_tMonster.HAction == HornetAction::Before_End)
		{
			if (GetComponent<Rigidbody>()->GetGround())
			{
				m_pLand->Play(false);
				if (m_tMonster.Direction == DIRECTION::Right)
					m_pHornetAni->Play(L"Hornet_Right_Sphere_End", false);
				if (m_tMonster.Direction == DIRECTION::Left)
					m_pHornetAni->Play(L"Hornet_Left_Sphere_End", false);
				m_tMonster.HAction = HornetAction::End;				
			}
		}

		if (m_tMonster.HAction == HornetAction::End)
		{
			s_fTimer += DT;
			if (s_fTimer > 0.2f)
			{
				if (m_vTargetPos.x > m_vMovePos.x)
					m_pHornetAni->Play(L"Hornet_Right_Standing", true);
				if (m_vTargetPos.x < m_vMovePos.x)
					m_pHornetAni->Play(L"Hornet_Left_Standing", true);

				m_tMonster.HAction = HornetAction::None;
				m_tMonster.HIA = HornetAI::End;
				s_fTimer = 0.f;
			}

		}



	}

	void Hornet::Evade()
	{
		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();

		if (m_tMonster.HAction == HornetAction::None)
		{
			//플에이어 위치가 오른쪽에 있다
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Left_Evade1", false);
				m_tMonster.Direction = DIRECTION::Left;
				m_tMonster.HAction = HornetAction::Evade;
			}
			//플에이어 위치가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Right_Evade1", false);
				m_tMonster.Direction = DIRECTION::Right;
				m_tMonster.HAction = HornetAction::Evade;
			}

		}
		if (m_tMonster.HAction == HornetAction::Evade)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.3f)
			{
				if (m_tMonster.Direction == DIRECTION::Left)
				{
					m_pHornetAni->Play(L"Hornet_Left_Evade2", false);
					m_pLeftEvade->Play(false);
				}
				if (m_tMonster.Direction == DIRECTION::Right)
				{
					m_pHornetAni->Play(L"Hornet_Right_Evade2", false);
					m_pRightEvade->Play(false);
				}
				m_tMonster.HAction = HornetAction::Walk;
				m_vAttackPos = GetPos();
				s_fTimer = 0.f;
			}
		}

		if (m_tMonster.HAction == HornetAction::Walk)
		{
			WindowData Resolution = Application::GetInstance().GetWindowData();

			if (m_tMonster.Direction == DIRECTION::Left)
			{
				//if (m_vAttackPos.x - 300 <= m_vMovePos.x && m_vMovePos.x <= (Resolution.width / Resolution.width) + 300)
				//if (m_vAttackPos.x - 300 <= m_vMovePos.x && m_vMovePos.x >= (Resolution.width / Resolution.width) + 300)
				if (m_vAttackPos.x - 300 <= m_vMovePos.x && !(m_bLeftWalkCheak))
					m_vMovePos.x -= 1200.f * DT;
				else
				{	
					m_pHornetAni->Play(L"Hornet_Right_Standing", true);
					m_tMonster.Direction = DIRECTION::Right;
					m_tMonster.HAction = HornetAction::None;
					m_tMonster.HIA = HornetAI::End;
				}
			}			
			if (m_tMonster.Direction == DIRECTION::Right)
			{
				//if (m_vAttackPos.x + 300 >= m_vMovePos.x && m_vMovePos.x >= (Resolution.width) - 300)
				//if (m_vAttackPos.x + 300 >= m_vMovePos.x && m_vMovePos.x <= (Resolution.width) - 300)
				if (m_vAttackPos.x + 300 >= m_vMovePos.x && !(m_bRightWalkCheak))
					m_vMovePos.x += 1200.f * DT;
				else
				{						
					m_pHornetAni->Play(L"Hornet_Left_Standing", true);
					m_tMonster.Direction = DIRECTION::Left;
					m_tMonster.HAction = HornetAction::None;
					m_tMonster.HIA = HornetAI::End;
				}
			}
			SetPos(m_vMovePos);
		}
	}





	void Hornet::HornetWalk()
	{
		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();
		WindowData Resolution = Application::GetInstance().GetWindowData();
		
		if(m_tMonster.HAction == HornetAction::None)
		{
			//플레이어가 오른쪽에 있다.
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				if (m_vTargetPos.x <= (Resolution.width / 2))
				{
					m_tMonster.Direction = DIRECTION::Right;
					m_pHornetAni->Play(L"Hornet_Right_Walk", true);
					m_tMonster.HAction = HornetAction::Walk;
					m_vAttackPos = GetPos();
					//m_Chaek = Cheak::Jump;
					//m_tMonster.HAction = HornetAction::None;						
					//HornetJump();
				}
				if (m_vTargetPos.x >= (Resolution.width / 2))
				{
					m_tMonster.Direction = DIRECTION::Left;
					m_pHornetAni->Play(L"Hornet_Left_Walk", true);
					m_tMonster.HAction = HornetAction::Walk;
					m_vAttackPos = GetPos();
				}
			}
			// 플레이어가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				if (m_vTargetPos.x >= (Resolution.width / 2))
				{
					m_tMonster.Direction = DIRECTION::Left;
					m_pHornetAni->Play(L"Hornet_Left_Walk", true);
					m_tMonster.HAction = HornetAction::Walk;
					m_vAttackPos = GetPos();
					//m_Chaek = Cheak::Jump;
					//m_tMonster.HAction = HornetAction::None;
					//HornetJump();
				}
				if (m_vTargetPos.x <= (Resolution.width / 2))
				{
					m_tMonster.Direction = DIRECTION::Right;
					m_pHornetAni->Play(L"Hornet_Right_Walk", true);
					m_tMonster.HAction = HornetAction::Walk;
					m_vAttackPos = GetPos();
				}

			}
		}

		if (m_tMonster.HAction == HornetAction::Walk)
		{
			if (m_tMonster.Direction == DIRECTION::Left)
			{
				//if(m_vMovePos.x >= (Resolution.width / Resolution.width) && m_vMovePos.x >= m_vAttackPos.x - 400)
				if(!(m_bLeftWalkCheak) && m_vMovePos.x >= m_vAttackPos.x - 400)
				m_vMovePos.x -= 300.f * DT;
				else
				{					
					m_tMonster.HAction = HornetAction::None;
					m_tMonster.HIA = HornetAI::End;
					if (m_vTargetPos.x > m_vMovePos.x)
						m_pHornetAni->Play(L"Hornet_Right_Standing", true);
					if (m_vTargetPos.x < m_vMovePos.x)
						m_pHornetAni->Play(L"Hornet_Left_Standing", true);
				}
			}
			if (m_tMonster.Direction == DIRECTION::Right)
			{
				//if (m_vMovePos.x <= (Resolution.width) && m_vMovePos.x <= m_vAttackPos.x + 400)
				if (!(m_bRightWalkCheak) && m_vMovePos.x <= m_vAttackPos.x + 400)
				m_vMovePos.x += 300.f * DT;
				else
				{
					m_tMonster.HAction = HornetAction::None;
					m_tMonster.HIA = HornetAI::End;
					if (m_vTargetPos.x > m_vMovePos.x)
						m_pHornetAni->Play(L"Hornet_Right_Standing", true);
					if (m_vTargetPos.x < m_vMovePos.x)
						m_pHornetAni->Play(L"Hornet_Left_Standing", true);
				}
			}			
			SetPos(m_vMovePos);

		}

	}

	void Hornet::HornetWalk(float distance)
	{
		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();

		WindowData Resolution = Application::GetInstance().GetWindowData();		

		if (m_tMonster.HAction == HornetAction::None)
		{
			m_pWalk->Play(true);
			//플레이어가 오른쪽에 있다.
			if (m_vTargetPos.x > m_vMovePos.x)
			{				
				m_pHornetAni->Play(L"Hornet_Right_Walk", true);
				m_tMonster.Direction = DIRECTION::Right;
				m_tMonster.HAction = HornetAction::Walk;
			}
			// 플레이어가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{				
				m_pHornetAni->Play(L"Hornet_Left_Walk", true);
				m_tMonster.Direction = DIRECTION::Left;
				m_tMonster.HAction = HornetAction::Walk;
			}
		}
		if (m_tMonster.HAction == HornetAction::Walk)
		{
			if (m_tMonster.Direction == DIRECTION::Left)
			{				
				//if (m_vMovePos.x > (Resolution.width / Resolution.width) && abs(m_vMovePos.x - m_vTargetPos.x) > distance)				
				if ((!m_bLeftWalkCheak) && abs(m_vMovePos.x - m_vTargetPos.x) > distance)
				{
					m_vMovePos.x -= 400.f * DT;					
				}
				else
				{
					m_pWalk->Stop(false);
					m_tMonster.HIA = (HornetAI)s_iRandom;
					m_tMonster.HAction = HornetAction::None;				
					if (m_vTargetPos.x > m_vMovePos.x)
						m_pHornetAni->Play(L"Hornet_Right_Standing", true);
					if (m_vTargetPos.x < m_vMovePos.x)
						m_pHornetAni->Play(L"Hornet_Left_Standing", true);
				}
			}
			if (m_tMonster.Direction == DIRECTION::Right)
			{				
				//if (m_vMovePos.x <= (Resolution.width) && abs(m_vMovePos.x - m_vTargetPos.x) > distance)				
				if ((!m_bRightWalkCheak) && abs(m_vMovePos.x - m_vTargetPos.x) > distance)
				{
					m_vMovePos.x += 400.f * DT;
				}
				else
				{
					m_pWalk->Stop(false);
					m_tMonster.HIA = (HornetAI)s_iRandom;
					m_tMonster.HAction = HornetAction::None;
					if (m_vTargetPos.x > m_vMovePos.x)
						m_pHornetAni->Play(L"Hornet_Right_Standing", true);
					if (m_vTargetPos.x < m_vMovePos.x)
						m_pHornetAni->Play(L"Hornet_Left_Standing", true);
				}
			}
			SetPos(m_vMovePos);
		}
	}

	void Hornet::HornetPushWalk(float distance)
	{
		if (m_tMonster.HAction == HornetAction::None)
		{
			//플레이어가 오른쪽에 있다.
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				//if (m_vMovePos.x > m_vAttackPos.x - distance)
				if (m_vMovePos.x > m_vAttackPos.x - distance && (!m_bLeftWalkCheak))
					m_vMovePos.x -= 1500.f * DT;
				else
				{		
					if (m_Hp <= 25 && m_bPlay == false)
					{
						m_tMonster.HIA = HornetAI::Phase;
						m_tMonster.HAction = HornetAction::None;
					}
					else if (m_Hp <= 17 && m_bChange == false)
					{
						m_tMonster.HIA = HornetAI::Phase;
						m_tMonster.HAction = HornetAction::None;
					}
					else
					m_tMonster = m_tHitSaveAi;
				}
			}
			// 플레이어가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				//if (m_vMovePos.x < m_vAttackPos.x + distance)
				if (m_vMovePos.x < m_vAttackPos.x + distance && (!m_bRightWalkCheak))
					m_vMovePos.x += 1500.f * DT;
				else
				{
					if (m_Hp <= 25 && m_bPlay == false)
					{
						m_tMonster.HIA = HornetAI::Phase;
						m_tMonster.HAction = HornetAction::None;
					}
					else if (m_Hp <= 17 && m_bChange == false)
					{
						m_tMonster.HIA = HornetAI::Phase;
						m_tMonster.HAction = HornetAction::None;
					}
					else
					m_tMonster = m_tHitSaveAi;
				}
			}
		}
				
		SetPos(m_vMovePos);
		
	}

	void Hornet::Hornet_Trap()
	{
		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();

		WindowData Resolution = Application::GetInstance().GetWindowData();

		if (m_tMonster.HAction == HornetAction::None)
		{
			//플레이어가 오른쪽에 있다.
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Right_Trap1", false);
				m_tMonster.Direction = DIRECTION::Right;
				m_tMonster.HAction = HornetAction::Trap;
			}
			// 플레이어가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Left_Trap1", false);
				m_tMonster.Direction = DIRECTION::Left;
				m_tMonster.HAction = HornetAction::Trap;
			}			
			m_pTrap->Play(false);
		}

		if (m_tMonster.HAction == HornetAction::Trap)
		{		
			s_fTimer += DT;
			if (s_fTimer >= 0.4f)
			{
				if (m_tMonster.Direction == DIRECTION::Right)
					m_pHornetAni->Play(L"Hornet_Right_Trap2", false);
				if (m_tMonster.Direction == DIRECTION::Left)
					m_pHornetAni->Play(L"Hornet_Left_Trap2", false);

				for (int i = 0; i < 3; ++i)
				{
					m_Attack = ya::object::Instantiate<Hornet_Attack>(eColliderLayer::Monster_Projecttile);
					//(최대 - 최소 + 1) + 최소
					Vector2 ThrPos = m_vTargetPos;
					Vector2 Max = {};

					int Col = rand() % ((Resolution.width / 2 - 30) - 50 + 1) + 50;
					int Rol = rand() % ((Resolution.height / 2 - 80) - 40 + 1) + 40;
					if (Col <= (int)(Resolution.width / 2 - 100) / 2)
						Col *= -1;					
						Rol *= -1;
					ThrPos += Vector2((float)Col, (float)Rol);

					m_Attack->SetAttack_Type(Attack_Type::Trap);
					m_Attack->SetTrap_Type(Tarp_Type::Thread);
					//m_Attack->SetPos(Vector2((float)Col, (float)Rol));
					m_Attack->SetPos(ThrPos);
				}
				
				
				m_tMonster.HAction = HornetAction::Before_End;
				s_fTimer = 0.f;
			}
		}
		if (m_tMonster.HAction == HornetAction::Before_End)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.4f)
			{
				if (m_tMonster.Direction == DIRECTION::Right)
					m_pHornetAni->Play(L"Hornet_Right_Trap3", false);
				if (m_tMonster.Direction == DIRECTION::Left)
					m_pHornetAni->Play(L"Hornet_Left_Trap3", false);
				
				m_tMonster.HAction = HornetAction::End;
				s_fTimer = 0.f;
			}			
		}

		if (m_tMonster.HAction == HornetAction::End)
		{
			s_fTimer += DT;
			if (s_fTimer > 0.5f)
			{
				if (m_vTargetPos.x > m_vMovePos.x)
					m_pHornetAni->Play(L"Hornet_Right_Standing", true);
				if (m_vTargetPos.x < m_vMovePos.x)
					m_pHornetAni->Play(L"Hornet_Left_Standing", true);

				m_pTrap1->Play(false);
				m_tMonster.HAction = HornetAction::None;
				m_tMonster.HIA = HornetAI::End;
				s_fTimer = 0.f;
			}

		}
		

	}

	void Hornet::Hornet_Defence()
	{
		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();	


		if (m_tMonster.HAction == HornetAction::None)
		{
			//플레이어가 오른쪽에 있다.
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Right_Counter_Start", false);
				m_tMonster.Direction = DIRECTION::Right;
				m_tMonster.HAction = HornetAction::Trap;

				m_pMonsterSize->SetScale(Vector2(110.0f, 170.0f));
				m_pMonsterSize->SetOffset(Vector2(40.f, 0.f));
			}
			// 플레이어가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Left_Counter_Start", false);
				m_tMonster.Direction = DIRECTION::Left;
				m_tMonster.HAction = HornetAction::Trap;

				m_pMonsterSize->SetScale(Vector2(110.0f, 170.0f));
				m_pMonsterSize->SetOffset(Vector2(-40.f, 0.f));
			}
		}

		if (m_tMonster.HAction == HornetAction::Trap)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.3f)
			{
				if (m_vTargetPos.x > m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Right_Counter_Hold", false);
					m_tMonster.Direction = DIRECTION::Right;
					m_tMonster.HAction = HornetAction::Play;
				}
				// 플레이어가 왼쪽에 있다
				if (m_vTargetPos.x < m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Left_Counter_Hold", false);
					m_tMonster.Direction = DIRECTION::Left;
					m_tMonster.HAction = HornetAction::Play;
				}
				s_fTimer = 0.f;
			}
		}

		if (m_tMonster.HAction == HornetAction::Play)
		{
			s_fTimer += DT;
			if (m_bCounter)
			{
				//막기에 성공 했을때
				if (m_vTargetPos.x > m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Right_Counter_Attack1", true);
					m_tMonster.Direction = DIRECTION::Right;
					m_tMonster.HAction = HornetAction::Counter;
					s_fTimer = 0.f;
				}
				
				if (m_vTargetPos.x < m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Left_Counter_Attack1", true);
					m_tMonster.Direction = DIRECTION::Left;
					m_tMonster.HAction = HornetAction::Counter;
					s_fTimer = 0.f;
				}
			}
			if (s_fTimer >= 3.0f)
			{
				//막기에 실패 했을때
				if (m_vTargetPos.x > m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Right_Counter_Fail", false);
					m_tMonster.Direction = DIRECTION::Right;	
					m_tMonster.HAction = HornetAction::Fail;
				}
				
				if (m_vTargetPos.x < m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Left_Counter_Fail", false);
					m_tMonster.Direction = DIRECTION::Left;
					m_tMonster.HAction = HornetAction::Fail;
				}
				
				m_pMonsterSize->SetScale(Vector2(180.0f, 220.0f));
				m_pMonsterSize->SetOffset(Vector2(0.0f, 0.0f));
				s_fTimer = 0.f;
			}

		}

		if (m_tMonster.HAction == HornetAction::Counter)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.3)
			{

				if (m_vTargetPos.x > m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Right_Counter_Attack2", false);
					m_tMonster.Direction = DIRECTION::Right;
					m_tMonster.HAction = HornetAction::Walk;
					m_pMonsterSize->SetScale(Vector2(180.0f, 340.0f));
					m_pMonsterSize->SetOffset(Vector2(200.0f, -80.0f));
				}

				if (m_vTargetPos.x < m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Left_Counter_Attack2", false);
					m_tMonster.Direction = DIRECTION::Left;
					m_tMonster.HAction = HornetAction::Walk;
					m_pMonsterSize->SetScale(Vector2(180.0f, 340.0f));
					m_pMonsterSize->SetOffset(Vector2(-200.0f, -80.0f));
				}
				m_pCounter1->Play(false);
				s_fTimer = 0.f;
			}
		}

		if (m_tMonster.HAction == HornetAction::Walk)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.12)
			{
				
				if (m_vTargetPos.x > m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Right_Counter_Attack3", false);
					m_tMonster.Direction = DIRECTION::Right;
					m_tMonster.HAction = HornetAction::Before_End;
					m_pMonsterSize->SetScale(Vector2(590.0f, 360));
					m_pMonsterSize->SetOffset(Vector2(0.0f, -70.0f));
				}

				if (m_vTargetPos.x < m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Left_Counter_Attack3", false);
					m_tMonster.Direction = DIRECTION::Left;
					m_tMonster.HAction = HornetAction::Before_End;
					m_pMonsterSize->SetScale(Vector2(590.0f, 360));
					m_pMonsterSize->SetOffset(Vector2(0.0f, -70.0f));
				}
				s_fTimer = 0.f;
			}
		}
		if (m_tMonster.HAction == HornetAction::Before_End)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.12)
			{
				if (m_vTargetPos.x > m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Right_Counter_Attack4", false);
					m_tMonster.Direction = DIRECTION::Right;
					m_tMonster.HAction = HornetAction::Fail;
				}

				if (m_vTargetPos.x < m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Left_Counter_Attack4", false);
					m_tMonster.Direction = DIRECTION::Left;
					m_tMonster.HAction = HornetAction::Fail;
				}
				m_pMonsterSize->SetScale(Vector2(180.0f, 220.0f));
				m_pMonsterSize->SetOffset(Vector2(0.0f, 0.0f));
				s_fTimer = 0.f;
			}
		}		


		//막기 실패 루트
		if (m_tMonster.HAction == HornetAction::Fail)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.2f)
			{
				if (m_vTargetPos.x > m_vMovePos.x)
					m_pHornetAni->Play(L"Hornet_Right_Standing", true);
				if (m_vTargetPos.x < m_vMovePos.x)
					m_pHornetAni->Play(L"Hornet_Left_Standing", true);

				m_tMonster.HAction = HornetAction::None;
				m_tMonster.HIA = HornetAI::End;
				m_bCounter = false;
				s_fTimer = 0.f;
			}
		}
	}


	void Hornet::Hornet_Stun()
	{

		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();
		
		CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, false);

		if (m_tMonster.HAction == HornetAction::None)
		{
			m_pStun->Play(false);
			//플레이어가 오른쪽에 있다.
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Right_Stun1", false);
				m_tMonster.Direction = DIRECTION::Right;
				m_tMonster.HAction = HornetAction::Walk;			
			}
			// 플레이어가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Left_Stun1", false);
				m_tMonster.Direction = DIRECTION::Left;
				m_tMonster.HAction = HornetAction::Walk;
			}
			m_vAttackPos = GetPos();
		}
		if (m_tMonster.HAction == HornetAction::Walk)
		{

			if (m_tMonster.Direction == DIRECTION::Right)
			{
				//if (m_vMovePos.x > m_vAttackPos.x - 60)
				if (m_vMovePos.x > m_vAttackPos.x - 60 && (!m_bLeftWalkCheak))
					m_vMovePos.x -= 100.f * DT;
				else
				{
					m_pHornetAni->Play(L"Hornet_Right_Stun2", false);
					m_tMonster.Direction = DIRECTION::Right;
					m_tMonster.HAction = HornetAction::Drop;
				}
			}
			if (m_tMonster.Direction == DIRECTION::Left)
			{
				//if (m_vMovePos.x < m_vAttackPos.x + 60)
				if (m_vMovePos.x < m_vAttackPos.x + 60 && (!m_bRightWalkCheak))
					m_vMovePos.x += 100.f * DT;
				else
				{
					m_pHornetAni->Play(L"Hornet_Left_Stun2", false);
					m_tMonster.Direction = DIRECTION::Left;
					m_tMonster.HAction = HornetAction::Drop;
				}
			}
			SetPos(m_vMovePos);
		}
		if (m_tMonster.HAction == HornetAction::Drop)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.4f)
			{
				if (m_vTargetPos.x > m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Right_Stun3", true);
					m_tMonster.Direction = DIRECTION::Right;
					m_tMonster.HAction = HornetAction::Stun;
				}
				// 플레이어가 왼쪽에 있다
				if (m_vTargetPos.x < m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Left_Stun3", true);
					m_tMonster.Direction = DIRECTION::Left;
					m_tMonster.HAction = HornetAction::Stun;
				}
				s_fTimer = 0.f;
			}
		}



		if (m_tMonster.HAction == HornetAction::Stun)
		{
			s_fTimer += DT;
			if (s_fTimer >= 3.0f)
			{
				if (m_vTargetPos.x > m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Right_Stun4", false);
					m_tMonster.Direction = DIRECTION::Right;
					m_tMonster.HAction = HornetAction::Play;
				}
				// 플레이어가 왼쪽에 있다
				if (m_vTargetPos.x < m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Left_Stun4", false);
					m_tMonster.Direction = DIRECTION::Left;
					m_tMonster.HAction = HornetAction::Play;
				}
				s_fTimer = 0.f;
			}

		}
		if (m_tMonster.HAction == HornetAction::Play)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.7f)
			{
				if (m_vTargetPos.x > m_vMovePos.x)
					m_pHornetAni->Play(L"Hornet_Right_Standing", true);
				if (m_vTargetPos.x < m_vMovePos.x)
					m_pHornetAni->Play(L"Hornet_Left_Standing", true);

				m_tMonster.HAction = HornetAction::None;
				m_tMonster.HIA = HornetAI::Play;
				s_fTimer = 0.f;
			}			
		}

	}





	void Hornet::Hornet_Play()
	{
		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();


		if (m_tMonster.HAction == HornetAction::None)
		{
			m_pStart->Play(false);
			//플레이어가 오른쪽에 있다.
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Right_Play", false);
				m_tMonster.Direction = DIRECTION::Right;
				m_tMonster.HAction = HornetAction::End;
			}
			// 플레이어가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Left_Play", false);
				m_tMonster.Direction = DIRECTION::Left;
				m_tMonster.HAction = HornetAction::End;
			}
		
		}

		if (m_tMonster.HAction == HornetAction::End)
		{
			s_fTimer += DT;
			if (s_fTimer >= 1.97f)
			{
				if (m_vTargetPos.x > m_vMovePos.x)
					m_pHornetAni->Play(L"Hornet_Right_Standing", true);
				if (m_vTargetPos.x < m_vMovePos.x)
					m_pHornetAni->Play(L"Hornet_Left_Standing", true);

				CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, true);
				m_tMonster.HAction = HornetAction::None;
				m_tMonster.HIA = HornetAI::End;
				if(m_Hp <= 25)
				m_bPlay = true;
				if(m_Hp <= 17)
				{
					m_bChange = true;
					m_iPhase += 4;
				}

				s_fTimer = 0.f;
			}
		}
	}

	void Hornet::Hornet_End()
	{

		if (m_pPlayer != nullptr)
			m_vTargetPos = m_pPlayer->GetPos();
		m_vMovePos = GetPos();

		CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, false);
		CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, false);

		if (m_tMonster.HAction == HornetAction::None)
		{
			m_pDeath->Play(false);
			//플레이어가 오른쪽에 있다.
			if (m_vTargetPos.x > m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Right_Stun1", false);
				m_tMonster.Direction = DIRECTION::Right;
				m_tMonster.HAction = HornetAction::Walk;
			}
			// 플레이어가 왼쪽에 있다
			if (m_vTargetPos.x < m_vMovePos.x)
			{
				m_pHornetAni->Play(L"Hornet_Left_Stun1", false);
				m_tMonster.Direction = DIRECTION::Left;
				m_tMonster.HAction = HornetAction::Walk;
			}
			m_vAttackPos = GetPos();
		}
		if (m_tMonster.HAction == HornetAction::Walk)
		{

			if (m_tMonster.Direction == DIRECTION::Right)
			{
				//if (m_vMovePos.x > m_vAttackPos.x - 60)
				if (m_vMovePos.x > m_vAttackPos.x - 60 && (!m_bLeftWalkCheak))
					m_vMovePos.x -= 100.f * DT;
				else
				{
					m_pHornetAni->Play(L"Hornet_Right_Stun2", false);
					m_tMonster.Direction = DIRECTION::Right;
					m_tMonster.HAction = HornetAction::Drop;
				}
			}
			if (m_tMonster.Direction == DIRECTION::Left)
			{
				//if (m_vMovePos.x < m_vAttackPos.x + 60)
				if (m_vMovePos.x < m_vAttackPos.x + 60 && (!m_bRightWalkCheak))
					m_vMovePos.x += 100.f * DT;
				else
				{
					m_pHornetAni->Play(L"Hornet_Left_Stun2", false);
					m_tMonster.Direction = DIRECTION::Left;
					m_tMonster.HAction = HornetAction::Drop;
				}
			}
			SetPos(m_vMovePos);
		}
		if (m_tMonster.HAction == HornetAction::Drop)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.4f)
			{
				if (m_vTargetPos.x > m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Right_Stun3", true);
					m_tMonster.Direction = DIRECTION::Right;
					m_tMonster.HAction = HornetAction::Stun;
				}
				// 플레이어가 왼쪽에 있다
				if (m_vTargetPos.x < m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Left_Stun3", true);
					m_tMonster.Direction = DIRECTION::Left;
					m_tMonster.HAction = HornetAction::Stun;
				}
				s_fTimer = 0.f;
			}
		}



		if (m_tMonster.HAction == HornetAction::Stun)
		{
			s_fTimer += DT;
			if (s_fTimer >= 3.0f)
			{
				if (m_vTargetPos.x > m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Right_Stun4", false);
					m_tMonster.Direction = DIRECTION::Right;
					m_tMonster.HAction = HornetAction::Play;
				}
				// 플레이어가 왼쪽에 있다
				if (m_vTargetPos.x < m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Left_Stun4", false);
					m_tMonster.Direction = DIRECTION::Left;
					m_tMonster.HAction = HornetAction::Play;
				}
				s_fTimer = 0.f;
			}

		}
		if (m_tMonster.HAction == HornetAction::Play)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.7f)
			{
				if (m_vTargetPos.x > m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Right_Standing", true);
					m_pHornetAniEffect->Play(L"Hornet_Last_Effect", false);
				}
				if (m_vTargetPos.x < m_vMovePos.x)
				{
					m_pHornetAni->Play(L"Hornet_Left_Standing", true);
					m_pHornetAniEffect->Play(L"Hornet_Last_Effect", false);
				}

				CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, true);
				CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, true);
				m_bHornet_Death = true;
				m_tMonster.HAction = HornetAction::None;
				m_tMonster.HIA = HornetAI::Death;
				s_fTimer = 0.f;
			}
		}


	}


}
