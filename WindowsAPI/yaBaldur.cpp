#include "yaBaldur.h"
#include "yaImage.h"
#include "yaTime.h"
#include "yaResources.h"
#include "yaAnimator.h"
#include "yaCollider.h"
#include "yaPlayer.h"
#include "yaRigidbody.h"
#include "yaPlayer_Attack.h"
#include "yaFalse_Key.h"
#include "yaObject.h"
#include "yaCollisionManager.h"
#include "yaSound.h"



namespace ya
{
	float Baldur::s_fTimer = 0.f;
	float Baldur::m_time = 0.f;

	Baldur::Baldur()
		: m_pBaldurImg(nullptr)
		, m_pBaldurAni(nullptr)
		, m_pPlayer(nullptr)
		, m_Sound(nullptr)
		, m_bTarget(false)
		, mbAniHit(false)
		, m_bLeftWalkCheak(false)
		, m_bRightWalkCheak(false)
		, m_Hp(3)
		, m_iAniHit(0)
		, m_pMonsterSize(nullptr)
		, m_vTargetPos(Vector2::Zero)
		, m_vFixPos(Vector2::Zero)
		, m_bFix(false)
	{
		SetName(L"Baldur");
		SetPos(GetPos());
		SetScale({ 1.0f, 1.0f });

		m_pBaldurImg = Resources::Load<Texture>(L"Baldur", L"..\\Resources\\Image\\Baldur.bmp");
		
		m_pBaldurAni = new Animator;

		m_tMonster.action = ACTION::End;
		m_tMonster.direction = DIRECTION::Right;

		m_pMonsterSize = new Collider();
		AddComponent(m_pMonsterSize);
		m_pMonsterSize->SetScale(Vector2(110.0f, 90.0f));
		
		if (nullptr == m_Sound)
			m_Sound = Resources::Load<Sound>(L"Monster_Damage_S", L"..\\Resources\\Sound\\Monster_Damage_S.wav");


		AnimationInitialize();



		m_pBaldurAni->Play(L"Baldur_Right_Standing", true);
		

		AddComponent(m_pBaldurAni);
		

		AddComponent<Rigidbody>();
		Rigidbody* rigidbody = GetComponent<Rigidbody>();
		rigidbody->SetGround(true);


	}

	Baldur::~Baldur()
	{

	}

	void Baldur::Tick()
	{
			if (nullptr != m_pPlayer)
			{
				m_vTargetPos = m_pPlayer->GetPos();
				Vector2 Area = GetPos();
				//if (abs(Area.x - m_vTargetPos.x) < 1000 && abs(Area.y - m_vTargetPos.y) < 1000)
				if (abs(Area.x - m_vTargetPos.x) < 800 && abs(Area.y - m_vTargetPos.y) < 400)
					m_bTar = true;
				else
					m_bTar = false;
			}

			if (m_bTar)
			{
				if (m_bFix == false)
				{
					m_vFixPos = GetPos();
					m_bFix = true;
				}
				switch (m_tMonster.action)
				{
				case ACTION::End:
				{ 
					m_vTargetPos = m_pPlayer->GetPos();
					Vector2 Area = GetPos();
					if (abs(Area.x - m_vTargetPos.x) < 800 && abs(Area.y - m_vTargetPos.y) < 400)
						m_tMonster.action = ACTION::None;
				}
				break;
				case ACTION::None:
				{
					if (m_tMonster.direction == DIRECTION::Left)
						m_pBaldurAni->Play(L"Baldur_Left_Standing", true);
					if (m_tMonster.direction == DIRECTION::Right)
						m_pBaldurAni->Play(L"Baldur_Right_Standing", true);

					m_vTargetPos = m_pPlayer->GetPos();
					m_tMonster.action = ACTION::Standing;
				
				}
				break;
				case ACTION::Standing:
				{
					s_fTimer += DT;
					if (s_fTimer >= 4.0f)
					{
						if (m_vFixPos.x == GetPos().x)
						{
							m_pBaldurAni->Play(L"Baldur_Left_Walk1", false);
							m_tMonster.direction = DIRECTION::Left;
						}
						if (m_vFixPos.x + 220 <= GetPos().x)
						{
							m_pBaldurAni->Play(L"Baldur_Left_Walk1", false);
							m_tMonster.direction = DIRECTION::Left;
						}
						if (m_vFixPos.x - 220 >= GetPos().x)
						{
							m_pBaldurAni->Play(L"Baldur_Right_Walk1", false);
							m_tMonster.direction = DIRECTION::Right;
						}
						m_tMonster.action = ACTION::Turn;
						s_fTimer = 0.f;
					}
				}
				break;
				case ACTION::Turn:
				{
					s_fTimer += DT;
					if (s_fTimer >= 0.4f)
					{
						if (m_tMonster.direction == DIRECTION::Left)
							m_pBaldurAni->Play(L"Baldur_Left_Walk2", true);
						if (m_tMonster.direction == DIRECTION::Right)
							m_pBaldurAni->Play(L"Baldur_Right_Walk2", true);

						m_pMonsterSize->SetScale(Vector2(80.0f, 70.0f));
						m_tMonster.action = ACTION::Walk;
						m_vMovePos = GetPos();
						s_fTimer = 0.f;
					}
				}
				break;
				case ACTION::Walk:
				{
					if (m_tMonster.direction == DIRECTION::Left)
					{			
						if(!m_bLeftWalkCheak)
						{
							m_vMovePos.x -= 200.f * DT;
							SetPos(m_vMovePos);
						}
						if (m_vMovePos.x <= m_vFixPos.x - 222)
						{
							m_pBaldurAni->Play(L"Baldur_Left_Stop", false);
							m_tMonster.action = ACTION::Attack_Cool;
							m_pMonsterSize->SetScale(Vector2(110.0f, 90.0f));
						}
					}
					if (m_tMonster.direction == DIRECTION::Right)
					{
						if (!m_bRightWalkCheak)
						{
							m_vMovePos.x += 200.f * DT;
							SetPos(m_vMovePos);
						}
						if (m_vMovePos.x >= m_vFixPos.x + 222)
						{
							m_pBaldurAni->Play(L"Baldur_Right_Stop", false);
							m_tMonster.action = ACTION::Attack_Cool;
							m_pMonsterSize->SetScale(Vector2(110.0f, 90.0f));
						}
					}
				}
				break;
				case ACTION::Attack_Cool:
				{
					s_fTimer += DT;
					if (s_fTimer >= 0.4f)
					{
						m_tMonster.action = ACTION::None;
						s_fTimer = 0.f;
					}
				}
				break;
				case ACTION::Detect:
				{
					m_vMovePos = GetPos();
					Vector2 direction = m_vTargetPos - m_vMovePos;
					direction.Normalize();
					if(direction.x > 0 && (!m_bRightWalkCheak))
					{
						m_vMovePos.x += direction.x * 120 * DT;
					}
					if (direction.x < 0 && (!m_bLeftWalkCheak))
					{
						m_vMovePos.x += direction.x * 120 * DT;
					}
					SetPos(m_vMovePos);

					if (!m_bTarget)
					{
						if (m_tMonster.direction == DIRECTION::Left)
							m_pBaldurAni->Play(L"Baldur_Left_Walk2", true);
						if (m_tMonster.direction == DIRECTION::Right)
							m_pBaldurAni->Play(L"Baldur_Right_Walk2", true);

						m_pMonsterSize->SetScale(Vector2(80.0f, 70.0f));
						m_bTarget = true;
					}
					//탐지 범위를 벗어나면 None 상태로 변경
					if (m_vMovePos.x - 401 > m_vTargetPos.x || m_vMovePos.y - 201 > m_vTargetPos.y
						|| m_vMovePos.x + 401 < m_vTargetPos.x || m_vMovePos.y + 100 < m_vTargetPos.y)
					{
						m_tMonster.action = ACTION::None;
						m_pMonsterSize->SetScale(Vector2(110.0f, 90.0f));
					}
					//조건 확인 후 탐지 영역 내에서 오른쪽 왼쪽으로 추적 or 타켓팅으로 변경
					Baldur_Right_Turn();
					Baldur_Left_Turn();
				}
				break;
				case ACTION::Target:
				{
					if (m_tMonster.direction == DIRECTION::Left)
					{
						if (m_vAttackPos.x - 500 <= m_vMovePos.x && (!m_bLeftWalkCheak))
							m_vMovePos.x -= 400.f * DT;
						else
						{
							m_pBaldurAni->Play(L"Baldur_Left_Stop", false);
							m_tMonster.action = ACTION::Attack_Cool;
							m_pMonsterSize->SetScale(Vector2(110.0f, 90.0f));
						}
					}
					if (m_tMonster.direction == DIRECTION::Right)
					{
						if (m_vAttackPos.x + 500 >= m_vMovePos.x && (!m_bRightWalkCheak))
							m_vMovePos.x += 400 * DT;
						else
						{
							m_pBaldurAni->Play(L"Baldur_Right_Stop", false);
							m_tMonster.action = ACTION::Attack_Cool;
							m_pMonsterSize->SetScale(Vector2(110.0f, 90.0f));
						}
					}
					SetPos(m_vMovePos);
				}
				break;
				case ACTION::Damage:
				{
					if (m_Hp == 0)
					{
						if (m_tMonster.direction == DIRECTION::Left)
						{
							if (m_vAttackPos.x + 50 >= m_vMovePos.x && (!m_bLeftWalkCheak))
								m_vMovePos.x += 350.f * DT;
							else
							{
								m_tMonster.action = ACTION::Attack;
								m_pBaldurAni->Play(L"Baldur_Right_Death1", false);
							}
						}
						if (m_tMonster.direction == DIRECTION::Right)
						{
							if (m_vAttackPos.x - 50 <= m_vMovePos.x && (!m_bRightWalkCheak))
								m_vMovePos.x -= 350.f * DT;
							else
							{
								m_tMonster.action = ACTION::Attack;
								m_pBaldurAni->Play(L"Baldur_Left_Death1", false);
							}					
						}						
					
					}
					else
					{
						//오른쪽 으로 밀려야 함..
						if (m_tMonster.direction == DIRECTION::Left)
						{
							if (m_vAttackPos.x + 100 >= m_vMovePos.x && (!m_bRightWalkCheak))
								m_vMovePos.x += 700.f * DT;
							else
								m_tMonster.action = ACTION::None;
						}
						//왼쪽 으로 밀려야 함..
						if (m_tMonster.direction == DIRECTION::Right)
						{
							if (m_vAttackPos.x - 100 <= m_vMovePos.x && (!m_bLeftWalkCheak))
								m_vMovePos.x -= 700.f * DT;
							else
								m_tMonster.action = ACTION::None;
						}
					}
					SetPos(m_vMovePos);
				}
				break;
				//맞고 죽는 모션임, 추가 하기 귀찮아서 어택으로 함
				case ACTION::Attack:
				{
					CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, false);
					CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, false);
					s_fTimer += DT;
					if (s_fTimer >= 0.3f)
					{
						if (m_tMonster.direction == DIRECTION::Left)
							m_pBaldurAni->Play(L"Baldur_Right_Death2", false);
						if (m_tMonster.direction == DIRECTION::Right)
							m_pBaldurAni->Play(L"Baldur_Left_Death2", false);
						m_tMonster.action = ACTION::Death_Air;
						s_fTimer = 0.f;

						m_vAttackPos = GetPos();
						m_vMovePos = GetPos();
					}
				}
				break;
				case ACTION::Death_Air:
				{
					s_fTimer = 0.f;
					CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, false);
					CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, false);
					if (m_tMonster.direction == DIRECTION::Left)
					{
						if (m_vAttackPos.x + 100 >= m_vMovePos.x && (!m_bRightWalkCheak))
							m_vMovePos.x += 400.f * DT;
						else
							m_tMonster.action = ACTION::Mortis;
					}
					//왼쪽 으로 밀려야 함..
					if (m_tMonster.direction == DIRECTION::Right)
					{
						if (m_vAttackPos.x - 100 <= m_vMovePos.x && (!m_bLeftWalkCheak))
							m_vMovePos.x -= 400.f * DT;
						else
							m_tMonster.action = ACTION::Mortis;
					}
					SetPos(m_vMovePos);



				}
				break;
				case ACTION::Mortis:
				{
					CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, false);
					CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, false);
					s_fTimer += DT;
					if (s_fTimer >= 1.5f)
					{
						if (m_tMonster.direction == DIRECTION::Left)
						{
							m_pBaldurAni->Play(L"Baldur_Right_Death3", false);
						}
						//왼쪽 으로 밀려야 함..
						if (m_tMonster.direction == DIRECTION::Right)
						{
							m_pBaldurAni->Play(L"Baldur_Left_Death3", false);
						}
						m_tMonster.action = ACTION::Death;	
						mbAniHit = true;
						s_fTimer = 0.f;
					}


				}
				break;
				case ACTION::Death:
				{
					CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, false);
					CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, false);

					s_fTimer += DT;
					if (s_fTimer >= 2.0f)
					{
					
						this->Death();
						False_Key* key = ya::object::Instantiate<False_Key>(eColliderLayer::GameObject);


						CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, true);
						CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, true);
						s_fTimer = 0.f;
					}
				}
				break;
				}


				m_vMovePos = GetPos();
				if (m_vTargetPos.x >= m_vMovePos.x - 400 && m_vTargetPos.y >= m_vMovePos.y - 200
					&& m_vTargetPos.x <= m_vMovePos.x + 400 && m_vTargetPos.y <= m_vMovePos.y + 100)
				{
					if (!(m_tMonster.action == ACTION::Detect) && !(m_tMonster.action == ACTION::Target)
						&& !(m_tMonster.action == ACTION::Attack) && !(m_tMonster.action == ACTION::Attack_Cool)
						&& !(m_tMonster.action == ACTION::Damage) && !(m_tMonster.action == ACTION::Death_Air)
						&& !(m_tMonster.action == ACTION::Death) && !(m_tMonster.action == ACTION::Mortis))
					{

						s_fTimer += DT;
						if (m_vTargetPos.x < m_vMovePos.x)
						{
							m_tMonster.direction = DIRECTION::Left;
							m_pBaldurAni->Play(L"Baldur_Left_Walk1", false);
							m_bTarget = false;
						}
						if (m_vTargetPos.x > m_vMovePos.x)
						{
							m_tMonster.direction = DIRECTION::Right;
							m_pBaldurAni->Play(L"Baldur_Right_Walk1", false);
							m_bTarget = false;
						}
						if (s_fTimer >= 0.4f)
						{
							m_tMonster.action = ACTION::Detect;
							s_fTimer = 0.f;
						}
					}
				}
			}

			else
			{
				m_tMonster.action = ACTION::None;
				m_pBaldurAni->Play(L"Baldur_Right_Standing", true);
			}


		


		GameObject::Tick();

	}

	void Baldur::Render(HDC hdc)
	{

		static float fRatio = 1.f;	//비율을 1부터 해줘서 캐릭터가 평소에는 보이게 해준다
		static float Dir = -1.f;

		if (mbAniHit)
		{
			m_time += DT;			//깜빡임 지속 시간 타이머
			fRatio += DT * Dir * 10.f;

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
			if (m_time > 2.0f)		//0.9초 후 다시 원래 상태로 돌림
			{
				mbAniHit = false;
				m_time = 0;
				fRatio = 1.f;
				Dir = -1.f;
			}
		}

		m_iAniHit = (int)((255) * (fRatio));

		GameObject::Render(hdc);

	}

	void Baldur::OnCollisionEnter(Collider* other)
	{

		if (Player_Attack* playerObj = dynamic_cast<Player_Attack*>(other->GetOwner()))
		{
			if(m_Hp > 0)
			{
				Vector2 DamagePos = m_vTargetPos;
				Vector2 Pos = GetPos();
				//오른쪽으로 밀려야 함..
				if (Pos.x > DamagePos.x)
					m_tMonster.direction = DIRECTION::Left;
				//왼쪽으로 미밀려야 함..
				if (Pos.x < DamagePos.x)
					m_tMonster.direction = DIRECTION::Right;

				m_vAttackPos = GetPos();
				m_vMovePos = GetPos();

				m_tMonster.action = ACTION::Damage;

				if (playerObj->GetSkill() == Player_Skill::Magic || playerObj->GetSkill() == Player_Skill::Magic_Walk)
				{
					playerObj->SetHit(true);
					if (m_Hp > 2)
					{
						m_Hp -= 3;
					}
					else if (m_Hp < 3)
					{
						m_Hp = 0;
					}
					m_Sound->Play(false);
				}
				else
				{
					m_Sound->Play(false);
					--m_Hp;
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

	void Baldur::OnCollisionStay(Collider* other)
	{
	}

	void Baldur::OnCollisionExit(Collider* other)
	{
	}

	void Baldur::AnimationInitialize()
	{
		//기본 서있는 모션
		m_pBaldurAni->CreateAnimation(L"Baldur_Left_Standing", m_pBaldurImg
			, Vector2(0.0f, 0.0f), Vector2(108.0f, 85.0f)
			, Vector2(0.0f, 0.0f), 4, 0.3f);
		m_pBaldurAni->CreateAnimation(L"Baldur_Right_Standing", m_pBaldurImg
			, Vector2(0.0f, 85.0f), Vector2(108.0f, 85.0f)
			, Vector2(0.0f, 0.0f), 4, 0.3f);

		//이동 모션 1 (몸 움크리기)
		m_pBaldurAni->CreateAnimation(L"Baldur_Left_Walk1", m_pBaldurImg
			, Vector2(0.0f, 170.0f), Vector2(125.0f, 100.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pBaldurAni->CreateAnimation(L"Baldur_Right_Walk1", m_pBaldurImg
			, Vector2(0.0f, 270.0f), Vector2(125.0f, 100.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);

		//이동 모션 2 (구르기)
		m_pBaldurAni->CreateAnimation(L"Baldur_Left_Walk2", m_pBaldurImg
			, Vector2(0.0f, 370.0f), Vector2(81.0f, 78.0f)
			, Vector2(0.0f, 0.0f), 3, 0.075f);
		m_pBaldurAni->CreateAnimation(L"Baldur_Right_Walk2", m_pBaldurImg
			, Vector2(0.0f, 448.0f), Vector2(81.0f, 78.0f)
			, Vector2(0.0f, 0.0f), 3, 0.075f);

		//이동 멈추고 대기 하는 모션
		m_pBaldurAni->CreateAnimation(L"Baldur_Left_Stop", m_pBaldurImg
			, Vector2(0.0f, 526.0f), Vector2(135.0f, 99.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		m_pBaldurAni->CreateAnimation(L"Baldur_Right_Stop", m_pBaldurImg
			, Vector2(0.0f, 625.0f), Vector2(135.0f, 99.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);

		//죽는 모션 1 (맞고 공중으로 뜨는 모션)
		m_pBaldurAni->CreateAnimation(L"Baldur_Right_Death1", m_pBaldurImg
			, Vector2(0.0f, 724.0f), Vector2(125.0f, 110.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);
		m_pBaldurAni->CreateAnimation(L"Baldur_Left_Death1", m_pBaldurImg
			, Vector2(0.0f, 834.0f), Vector2(125.0f, 110.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);


		//죽는 모션 2 (땅에 딱 몸 웅크리고 있는 모션)
		m_pBaldurAni->CreateAnimation(L"Baldur_Left_Death2", m_pBaldurImg
			, Vector2(243.0f, 370.0f), Vector2(77.0f, 77.0f)
			, Vector2(0.0f, 0.0f), 1, 0.1f);
		m_pBaldurAni->CreateAnimation(L"Baldur_Right_Death2", m_pBaldurImg
			, Vector2(243.0f, 448.0f), Vector2(77.0f, 77.0f)
			, Vector2(0.0f, 0.0f), 1, 0.1f);

		//죽는 모션 3
		m_pBaldurAni->CreateAnimation(L"Baldur_Right_Death3", m_pBaldurImg
			, Vector2(0.0f, 944.0f), Vector2(133.0f, 112.0f)
			, Vector2(0.0f, 0.0f), 5, 0.2f);
		m_pBaldurAni->CreateAnimation(L"Baldur_Left_Death3", m_pBaldurImg
			, Vector2(0.0f, 1056.0f), Vector2(133.0f, 112.0f)
			, Vector2(0.0f, 0.0f), 5, 0.2f);


	}
	void Baldur::Baldur_Left_Turn()
	{
		//m_vMovePos = GetPos();

		if (m_tMonster.direction == DIRECTION::Right)
		{
			if (m_vMovePos.x + 300 >= m_vTargetPos.x)
			{
				//공격 범위 안으로 진입하면 타겟팅 상태로 변경
				m_vAttackPos = GetPos();
				m_tMonster.action = ACTION::Target;
			}

			if (m_vMovePos.x - 100 > m_vTargetPos.x)
			{
				//플레이어가 반대편으로 돌아가면 반대로 이동
				m_pBaldurAni->Play(L"Baldur_Left_Walk1", false);
				s_fTimer += DT;
				if (s_fTimer > 0.4f)
				{
					m_tMonster.direction = DIRECTION::Left;
					m_tMonster.action = ACTION::Turn;
					m_bTarget = false;
					s_fTimer = 0.f;
				}
			}
		}

	}
	void Baldur::Baldur_Right_Turn()
	{
		if (m_tMonster.direction == DIRECTION::Left)
		{
			if (m_vMovePos.x - 300 <= m_vTargetPos.x)
			{
				//공격 범위 안으로 진입하면 타겟팅 상태로 변경
				m_vAttackPos = GetPos();
				m_tMonster.action = ACTION::Target;
			}

			if (m_vMovePos.x + 100 < m_vTargetPos.x)
			{
				//플레이어가 반대편으로 돌아가면 반대로 이동
				m_pBaldurAni->Play(L"Baldur_Right_Walk1", false);
				s_fTimer += DT;
				if (s_fTimer > 0.4f)
				{
					m_tMonster.direction = DIRECTION::Right;
					m_tMonster.action = ACTION::Turn;
					m_bTarget = false;
					s_fTimer = 0.f;
				}
			}
		}
	}
}