#include "yaPlayer.h"
#include "yaTime.h"
#include "yaSceneManager.h"
#include "yaInput.h"
#include "yaMissile.h"
#include "yaScene.h"
#include "yaImage.h"
#include "yaCamera.h"
#include "yaHusk.h"
#include "yaBaldur.h"
#include "yaCollisionManager.h"
#include "yaHornet.h"
#include "yaObject.h"
#include "yaHornet_Attack.h"
#include "yaResources.h"
#include "yaAnimator.h"
#include "yaCollider.h"
#include "yaCamera.h"
#include "yaPlayer_Attack.h"
#include "yaBackPack.h"
#include "yaRigidbody.h"
#include "yaFalse_Knight.h"
#include "yaFalse_Attack.h"
#include "yaUIManager.h"
#include "yaSound.h"

// === GDI+ ===
#include <ole2.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
// ============

using namespace Gdiplus;
ULONG_PTR gdiplusToken = NULL;

namespace ya
{
	float Player::s_fCheakTime = 0.f;
	float Player::s_fTimer = 0.f;
	float Player::m_time = 0.f;
	float Player::s_fSkillTimer = 0.f;
	Vector2 Player::m_vFixPos = Vector2::Zero;
	bool Player::m_bInvincibility = false;
	bool Player::m_bSave = false;
	bool Player::m_bStart = false;


	Player::Player()
		: m_pStateImg(nullptr)
		, m_pPlayerDashEffect(nullptr)
		, m_pPlayerAttackeffect(nullptr)
		, m_pPlayerAttack_Up_Down_effect(nullptr)
		, m_pPlayerHit(nullptr)
		, m_pSAttack(nullptr)
		, m_pSMagic(nullptr)
		, m_pSDash(nullptr)
		, m_pSWalk(nullptr)
		, m_pSJump(nullptr)
		, m_pSLand(nullptr)
		, m_pSFloating(nullptr)
		, m_pSHeal(nullptr)
		, m_pSHeal1(nullptr)
		, m_pSHit(nullptr)
		, m_pDeath(nullptr)
		, m_DashAir(false)
		, m_bHitAir(false)
		, m_AttackAri(false)
		, mbAniHit(false)
		, m_bHp(false)
		, m_bPlayerIn(false)
		, m_bMagic(false)
		, m_bLeftWalkCheak(false)
		, m_bRightWalkCheak(false)
		, m_bHealSave(false)
		, mSpeed(1.0f)
		, mHp(5)
		, mMp(0)
		, m_iAniHit(0)
	{
		SetName(L"Player");
		SetPos({ 1000.0f, 500.0f });
		SetScale({ 1.0f, 1.0f });

		mAnimator = new Animator();
		mAttackAnimator = new Animator();
		
		GdiplusStartupInput  startup;
		GdiplusStartup(&gdiplusToken, &startup, NULL);

		TextureInitialize();
		AnimationInitialize();
		SoundInitialize();

		AddComponent(mAnimator);
		AddComponent(mAttackAnimator);
	
		
		//mAnimator->Play(L"Player_Sit_Up1", false);
		//mAttackAnimator->Play(L"Player_Cure_Effect", true);
		m_tPlayer.situation = SITUATION::Stading;
		m_tPlayer.bgm = PLAYER_BGM::Back;


		Collider* coliider = new Collider();
		AddComponent(coliider);

		coliider->SetOffset(Vector2(0.0f, 0.0f));
		coliider->SetScale(Vector2(70.0f, 120.0f));


		//Rigidbody* rigidBody = new Rigidbody();
		//AddComponent(rigidBody);

		AddComponent<Rigidbody>();
		Rigidbody* rigidbody = GetComponent<Rigidbody>();
		rigidbody->SetGround(true);
	}

	Player::~Player()
	{
		//GDI+ 메모리 해제
		GdiplusShutdown(gdiplusToken);
	}

	void Player::Tick()
	{
		GameObject::Tick();
		
		m_tPlayer.Pos = GetPos();

		if (m_bStart == false)
		{
			s_fCheakTime += DT;
			if (s_fCheakTime >= 1)
			{
				Rigidbody* rigidbody = GetComponent<Rigidbody>();
				rigidbody->SetGround(false);
				m_bStart = true;
				s_fCheakTime = 0.f;
			}
		}


		if (m_bSave)
		{
			m_vFixPos = GetPos();
			m_bSave = false;
		}
		if (m_bPlayerIn)
		{
			s_fTimer += DT;
			if (s_fTimer >= 1.0 && s_fTimer <= 1.05)
			{
				Camera::SetCameraEffect(eCameraEffect::FadeOut);
			}
			if (s_fTimer >= 3.96 && s_fTimer <= 4.0)
			{				
				Camera::SetCameraEffect(eCameraEffect::FadeIn);
			}
			m_time += DT;
			if(m_time >= 0.01 && m_time <= 0.05)
			{
				m_tPlayer.situation = SITUATION::In;
				mAnimator->Play(L"Player_In", true);				
			}
		
			if(m_time >= 4.0)
			{
				m_bPlayerIn = false;
				m_tPlayer.situation = SITUATION::End;
				m_time = 0.f;
				s_fTimer = 0.f;
			}
		}

		if (m_bHealSave)
		{
			s_fTimer += DT;
			if (s_fTimer >= 0.1 && s_fTimer <= 0.13)
			{
				mAnimator->Play(L"Player_Sit_Up1", false);				
			}
			if (s_fTimer >= 0.13 && s_fTimer <= 0.73)
			{			
				Rigidbody* rigidbody = GetComponent<Rigidbody>();
				rigidbody->SetGround(true);
				m_tPlayer.situation = SITUATION::Save;
				Vector2 Pos = GetPos();
				Pos.y -= 50.0f * DT;
				SetPos(Pos);
			}
			if (s_fTimer >= 0.6 && s_fTimer <= 0.61)
			{
				mAttackAnimator->Play(L"Player_Cure_Effect", false);
				mHp = 5;
				mMp = 0;
				m_bSave = true;
				m_bHealSave = false;				
				s_fTimer = 0.f;
			}			
			//m_tPlayerAction = Player_Action::None;
			//m_tPlayer.situation = SITUATION::End;

		}

		if (m_tPlayer.situation == SITUATION::Save)
		{
			if (KEY_DOWN(eKeyCode::LEFT))
			{ 
				Rigidbody* rigidbody = GetComponent<Rigidbody>();
				rigidbody->SetGround(false);
				m_tPlayerAction = Player_Action::None;
				m_tPlayer.situation = SITUATION::End;
			}
			if (KEY_DOWN(eKeyCode::RIGHT))
			{ 
				Rigidbody* rigidbody = GetComponent<Rigidbody>();
				rigidbody->SetGround(false);
				m_tPlayerAction = Player_Action::None;
				m_tPlayer.situation = SITUATION::End;
			}
		}


		switch (m_tPlayerAction)
		{
		case ya::Player_Action::None:
		{

		}
			break;
		case ya::Player_Action::Death:
		{
			CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, false);
			
			s_fTimer += DT;
			if (s_fTimer >= 0.9 && s_fTimer <= 0.95)
			{
				m_pDeath->Play(false);
				mAnimator->Play(L"Player_Death", false);
			}
			if (s_fTimer >= 2.0 && s_fTimer <= 2.05)
			{
				Camera::SetCameraEffect(eCameraEffect::FadeOut);
			}
			if (s_fTimer >= 4.96 && s_fTimer <= 5.0)
			{
				CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, true);
				this->SetPos(m_vFixPos);
				mAnimator->Play(L"Right_Standing", false);
				mHp = 5;
				mMp = 0;
				s_fTimer = 0.f;
				m_tPlayerAction = Player_Action::None;
				Camera::SetCameraEffect(eCameraEffect::FadeIn);
			}

		}
			break;
		case ya::Player_Action::Walk:
		{

		}
			break;
		case ya::Player_Action::End:
		{

		}
			break;		
		}


		m_tPlayer.Pos = GetPos();

		if (m_tPlayer.situation == SITUATION::End || m_tPlayer.situation == SITUATION::Stading)
		{
			if (m_tPlayer.direction == DIRECTION::Left)
				mAnimator->Play(L"Left_Standing", true);
			if (m_tPlayer.direction == DIRECTION::Right)
				mAnimator->Play(L"Right_Standing", true);

			m_tPlayer.situation = SITUATION::None;
		}

		//다른 행동 불가 시간 만큼 지연 걸어주고, 각 행동별로 끝날때 상태값 저장
		if (m_tPlayer.Limit_Cheak)
		{			
			s_fCheakTime += DT;
			if (m_tPlayer.Time_Limit <= s_fCheakTime)
			{
				if (m_tPlayer.situation == SITUATION::Dash)				
				{
					Rigidbody* rigidbody = GetComponent<Rigidbody>();
					rigidbody->SetVelocity(Vector2::Zero);
					if(m_DashAir)
					rigidbody->SetGround(false);
					if(m_bDashInvinci)
					{
						m_bInvincibility = false;
						m_bDashInvinci = false;
					}
					m_tPlayer.situation = SITUATION::End;
				}
				if (m_tPlayer.situation == SITUATION::Hit)				
				{
					m_tPlayer.situation = SITUATION::End;
				}
				s_fCheakTime = 0.f;
				m_tPlayer.Limit_Cheak = false;
			}
		}

		//공격 모션 연속으로 눌러도 일정 시간 이후 공격가능하게 조건
		if (m_tPlayer.situation == SITUATION::Attack
			|| m_tPlayer.situation == SITUATION::Up_Attack || m_tPlayer.situation == SITUATION::Down_Attack)
		{
			s_fCheakTime += DT;
			if (s_fCheakTime >= 0.28f)
			{				
				
				m_tPlayer.situation = SITUATION::End;
				s_fCheakTime = 0.f;
			}
		}

		// ==========무적 모드 ==========
		if (KEY_DOWN(eKeyCode::N_2))
		{
			if (m_bInvincibility)
				m_bInvincibility = false;
			else
				m_bInvincibility = true;
		}


		// ==========이동 커맨드 ==========
		
		if (KEY_PRESSE(eKeyCode::LEFT))
		{		
			if (!m_tPlayer.Limit_Cheak && !m_bLeftWalkCheak)
			{				
				mPlayerPos = GetPos();
				mPlayerPos.x -= 550.0f * DT;
				SetPos(mPlayerPos);
			}
		}
		
		if (KEY_PRESSE(eKeyCode::RIGHT))
		{
			if (!m_tPlayer.Limit_Cheak && !m_bRightWalkCheak)
			{		
				mPlayerPos = GetPos();
				mPlayerPos.x += 550.0f * DT;
				SetPos(mPlayerPos);
			}
		}

		if (KEY_DOWN(eKeyCode::LEFT))
		{
			if (!m_tPlayer.Limit_Cheak)
			{
				if (GetComponent<Rigidbody>()->GetGround())
				{
					if (m_tPlayer.situation == SITUATION::Walk)
					{
						mAnimator->Play(L"Right_Standing", true);
						m_tPlayer.situation = SITUATION::both;
						m_tPlayer.direction = DIRECTION::Right;
					}
					else
					{
						mAnimator->Play(L"Left_Walk", true);
						m_tPlayer.situation = SITUATION::Walk;
						m_tPlayer.direction = DIRECTION::Left;
						m_pSWalk->Play(true);
					}
				}
				else
				{
					m_tPlayer.direction = DIRECTION::Left;
				}
			}
		}
		if (KEY_DOWN(eKeyCode::RIGHT))
		{
			if (!m_tPlayer.Limit_Cheak)
			{
				if (GetComponent<Rigidbody>()->GetGround())
				{
					if (m_tPlayer.situation == SITUATION::Walk)
					{
						mAnimator->Play(L"Left_Standing", true);
						m_tPlayer.situation = SITUATION::both;
						m_tPlayer.direction = DIRECTION::Left;
					}
					else
					{
						mAnimator->Play(L"Right_Walk", true);
						m_tPlayer.situation = SITUATION::Walk;
						m_tPlayer.direction = DIRECTION::Right;
						m_pSWalk->Play(true);
					}
				}

				else
				{
					m_tPlayer.direction = DIRECTION::Right;
				}
			}
		}

		if (KEY_UP(eKeyCode::LEFT))
		{
			if (!m_tPlayer.Limit_Cheak)
			{
				if (GetComponent<Rigidbody>()->GetGround())
				{
					if (m_tPlayer.situation == SITUATION::both)
					{
						mAnimator->Play(L"Right_Walk", true);
						m_tPlayer.situation = SITUATION::Walk;
						m_tPlayer.direction = DIRECTION::Right;
					}
					else
					{
						mAnimator->Play(L"Left_Standing", true);
						m_tPlayer.situation = SITUATION::Stading;
						m_tPlayer.direction = DIRECTION::Left;
						m_pSWalk->Stop(true);
					}
				}
				else
				{
					m_tPlayer.direction = DIRECTION::Left;
				}
			}
		}
		if (KEY_UP(eKeyCode::RIGHT))
		{
			if (!m_tPlayer.Limit_Cheak)
			{
				if (GetComponent<Rigidbody>()->GetGround())
				{
					if (m_tPlayer.situation == SITUATION::both)
					{
						mAnimator->Play(L"Left_Walk", true);
						m_tPlayer.situation = SITUATION::Walk;
						m_tPlayer.direction = DIRECTION::Left;
					}
					else
					{
						m_pSWalk->Stop(true);
						mAnimator->Play(L"Right_Standing", true);
						m_tPlayer.situation = SITUATION::Stading;
						m_tPlayer.direction = DIRECTION::Right;
					}
				}
				else
				{
					m_tPlayer.direction = DIRECTION::Right;
				}
			}
		}
		// ==========행동 커맨드 ==========
		if (KEY_DOWN(eKeyCode::UP))
		{
			if (!(m_tPlayer.Limit_Cheak))
			{
							
				mUpDown_Pos = Camera::GetCameraLook();
				mUpDownStop_Pos.y = mUpDown_Pos.y - 400;

				if (m_tPlayer.direction == DIRECTION::Left && GetComponent<Rigidbody>()->GetGround())
					mAnimator->Play(L"Left_Up_down", false);
				if (m_tPlayer.direction == DIRECTION::Right && GetComponent<Rigidbody>()->GetGround())
					mAnimator->Play(L"Right_Up_down", false);

				m_tPlayer.situation = SITUATION::Look_Up;
			}
		}
	
		if (KEY_DOWN(eKeyCode::DOWN))
		{
			if (!(m_tPlayer.Limit_Cheak))
			{					
					mUpDown_Pos = Camera::GetCameraLook();
					mUpDownStop_Pos.y = mUpDown_Pos.y + 400;

					if (m_tPlayer.direction == DIRECTION::Left && GetComponent<Rigidbody>()->GetGround())
						mAnimator->Play(L"Left_Down_down", false);
					if (m_tPlayer.direction == DIRECTION::Right && GetComponent<Rigidbody>()->GetGround())
						mAnimator->Play(L"Right_Down_down", false);
				
						m_tPlayer.situation = SITUATION::Look_Down;
			}
		}


		if (KEY_PRESSE(eKeyCode::UP))
		{
			if (!(m_tPlayer.Limit_Cheak))
			{
				m_tPlayer.attack = Attack::Up;
				if (m_tPlayer.situation == SITUATION::Look_Up)
				{
					s_fCheakTime += DT;
					if (s_fCheakTime >= 0.5f)
					{
						if (m_tPlayer.direction == DIRECTION::Left)
							mAnimator->Play(L"Left_Up_presse", true);
						if (m_tPlayer.direction == DIRECTION::Right)
							mAnimator->Play(L"Right_Up_presse", true);
						m_tPlayer.situation = SITUATION::Up_Presse;
						s_fCheakTime = 0.f;
					}					
				}

				if(m_tPlayer.situation == SITUATION::Up_Presse)
				{
					s_fCheakTime += DT;
					if (s_fCheakTime >= 1.0f)
					{
							m_tPlayer.situation = SITUATION::Up_End;
							s_fCheakTime = 0.f;
					}
				}
				if (m_tPlayer.situation == SITUATION::Up_End)
				{
					Vector2 Pos = Camera::GetCameraLook();
	
					if (mUpDownStop_Pos.y >= Pos.y)
					{
						if (m_tPlayer.direction == DIRECTION::Left)
							mAnimator->Play(L"Left_Up_presse", true);
						if (m_tPlayer.direction == DIRECTION::Right)
							mAnimator->Play(L"Right_Up_presse", true);
					}
					else
					{
						Pos.y -= 400.f * DT;
						Camera::SetCameraLook(Pos);
					}
				}				
			}

		}
		if (KEY_PRESSE(eKeyCode::DOWN))
		{
			if (!(m_tPlayer.Limit_Cheak))
			{
				m_tPlayer.attack = Attack::Down;
				if (m_tPlayer.situation == SITUATION::Look_Down)
				{
					s_fCheakTime += DT;
					if (s_fCheakTime >= 0.5f)
					{
						if (m_tPlayer.direction == DIRECTION::Left)
							mAnimator->Play(L"Left_Down_presse", true);
						if (m_tPlayer.direction == DIRECTION::Right)
							mAnimator->Play(L"Right_Down_presse", true);

						m_tPlayer.situation = SITUATION::Down_Presse;
					}
				}

				if (m_tPlayer.situation == SITUATION::Down_Presse)
				{
					s_fCheakTime += DT;
					if (s_fCheakTime >= 1.0f)
					{
						m_tPlayer.situation = SITUATION::Down_End;
						s_fCheakTime = 0.f;
					}
				}

				if (m_tPlayer.situation == SITUATION::Down_End)
				{
					Vector2 Pos = Camera::GetCameraLook();

					if (mUpDownStop_Pos.y <= Pos.y)
					{
						if (m_tPlayer.direction == DIRECTION::Left)
							mAnimator->Play(L"Left_Down_presse", true);
						if (m_tPlayer.direction == DIRECTION::Right)
							mAnimator->Play(L"Right_Down_presse", true);
					}
					else
					{
						Pos.y += 400.f * DT;
						Camera::SetCameraLook(Pos);
					}
				}
			}
		}

		if (KEY_UP(eKeyCode::UP))
		{			
			if (!(m_tPlayer.Limit_Cheak))
			{

				if (m_tPlayer.situation == SITUATION::Up_End || m_tPlayer.situation == SITUATION::Up_Presse
					|| m_tPlayer.situation == SITUATION::Look_Up)
				{
					if (m_tPlayer.direction == DIRECTION::Left)
					{
						mAnimator->Play(L"Left_Up_up", false);
						m_tPlayer.situation = SITUATION::End;
						Camera::SetCameraLook(Vector2(mUpDown_Pos.x, mUpDown_Pos.y));
					}

					if (m_tPlayer.direction == DIRECTION::Right)
					{
						mAnimator->Play(L"Right_Up_up", false);
						m_tPlayer.situation = SITUATION::End;
						Camera::SetCameraLook(Vector2(mUpDown_Pos.x, mUpDown_Pos.y));
					}
				}
			}
			m_tPlayer.attack = Attack::None;
		}


		if (KEY_UP(eKeyCode::DOWN))
		{
			if (!(m_tPlayer.Limit_Cheak))
			{
				if (m_tPlayer.situation == SITUATION::Down_End || m_tPlayer.situation == SITUATION::Down_Presse
					|| m_tPlayer.situation == SITUATION::Look_Down)
				{
					if (m_tPlayer.direction == DIRECTION::Left)
					{
						mAnimator->Play(L"Left_Down_up", false);
						m_tPlayer.situation = SITUATION::End;
						Camera::SetCameraLook(Vector2(mUpDown_Pos.x, mUpDown_Pos.y));
					}
					if (m_tPlayer.direction == DIRECTION::Right)
					{
						mAnimator->Play(L"Right_Down_up", false);
						m_tPlayer.situation = SITUATION::None;
						Camera::SetCameraLook(Vector2(mUpDown_Pos.x, mUpDown_Pos.y));
					}
				}				
			}
			m_tPlayer.attack = Attack::None;
		}


	


		if (KEY_DOWN(eKeyCode::C))
		{
			if ((!m_tPlayer.Limit_Cheak) && !(m_DashAir))
			{
				m_pSDash->Play(false);
				if (m_bInvincibility == false)
				{
					m_bInvincibility = true;
					m_bDashInvinci = true;
				}
				m_tPlayer.Limit_Cheak = true;
				if (m_tPlayer.direction == DIRECTION::Left)
				{
					mAnimator->Play(L"PlayerLeft_Dash", false);
					mAttackAnimator->Play(L"Left_DashEffect", false);
					Rigidbody* rigidbody = GetComponent<Rigidbody>();
					rigidbody->SetVelocity(Vector2(-1000.0f, 0.0f));
					if (!(rigidbody->GetGround()))
					{
						rigidbody->SetGround(true);
						m_DashAir = true;
					}
					m_tPlayer.situation = SITUATION::Dash;
					m_tPlayer.direction = DIRECTION::Left;
				}
				if (m_tPlayer.direction == DIRECTION::Right)
				{					
					mAnimator->Play(L"PlayerRight_Dash", false);
					mAttackAnimator->Play(L"Right_DashEffect", false);
					Rigidbody* rigidbody = GetComponent<Rigidbody>();
					rigidbody->SetVelocity(Vector2(+1000.0f, 0.0f));
					if (!(rigidbody->GetGround()))
					{
						rigidbody->SetGround(true);
						m_DashAir = true;
					}
					m_tPlayer.situation = SITUATION::Dash;
					m_tPlayer.direction = DIRECTION::Right;
				}
				m_tPlayer.Time_Limit = 0.5f;
			}
		}
		//점프 모션 들어갈때 (점프1)
		if (KEY_DOWN(eKeyCode::X))
		{
			if (!m_tPlayer.Limit_Cheak)
			{
				if (!(m_tPlayer.situation == SITUATION::Jump) && !(m_tPlayer.situation == SITUATION::Weightless)
					&& !(m_tPlayer.situation == SITUATION::Floating) && !(m_tPlayer.situation == SITUATION::Gravity))
				{
					m_pSWalk->Stop(true);
					m_pSJump->Play(false);
					if (m_tPlayer.direction == DIRECTION::Right)
					{
						mAnimator->Play(L"Right_Jump1", false);
						m_tPlayer.situation = SITUATION::Jump;
						m_tPlayer.direction = DIRECTION::Right;
					}
					if (m_tPlayer.direction == DIRECTION::Left)
					{
						mAnimator->Play(L"Left_Jump1", false);
						m_tPlayer.situation = SITUATION::Jump;
						m_tPlayer.direction = DIRECTION::Left;
					}

					Vector2 pos = GetPos();

					mJumpStopPos.y = pos.y - 200.f;
				}
			}
		}
		//점프 모션 중일때 (점프2)
		if (KEY_PRESSE(eKeyCode::X))
		{
			if (!m_tPlayer.Limit_Cheak)
			{
				if (m_tPlayer.situation == SITUATION::Jump || m_tPlayer.situation == SITUATION::Weightless)
				{
					s_fCheakTime += DT;
					if (s_fCheakTime >= 0.2f)
					{
						if (m_tPlayer.direction == DIRECTION::Right)
						{
							mAnimator->Play(L"Right_Jump2", true);
							m_tPlayer.situation = SITUATION::Weightless;
							m_tPlayer.direction = DIRECTION::Right;
						}
						if (m_tPlayer.direction == DIRECTION::Left)
						{
							mAnimator->Play(L"Left_Jump2", true);
							m_tPlayer.situation = SITUATION::Weightless;
							m_tPlayer.direction = DIRECTION::Left;
						}
						s_fCheakTime = 0.f;
					}

					Rigidbody* rigidbody = GetComponent<Rigidbody>();
					Vector2 velocity = rigidbody->GetVelocity();
					velocity.y = -500.0f;
					rigidbody->SetVelocity(velocity);
					rigidbody->SetGround(false);
					if (GetPos().y <= mJumpStopPos.y)
					{
						m_pSFloating->Play(false);
						m_tPlayer.situation = SITUATION::Floating;
					}


				}
			}
		}
		//점프 키에서 손을 떼었을때
		if (KEY_UP(eKeyCode::X))
		{
			if (!m_tPlayer.Limit_Cheak)
				if (!(GetComponent<Rigidbody>()->GetGround()) && m_tPlayer.situation == SITUATION::Jump
					|| m_tPlayer.situation == SITUATION::Weightless)
				{
					m_pSFloating->Play(false);
					if (m_tPlayer.direction == DIRECTION::Right)
					{
						mAnimator->Play(L"Right_Jump3", false);
						m_tPlayer.situation = SITUATION::Floating;
						m_tPlayer.direction = DIRECTION::Right;
					}
					if (m_tPlayer.direction == DIRECTION::Left)
					{
						mAnimator->Play(L"Left_Jump3", false);
						m_tPlayer.situation = SITUATION::Floating;
						m_tPlayer.direction = DIRECTION::Left;
					}
				}
		}

		//낙하 하는 중 애니메이션

		if (!(GetComponent<Rigidbody>()->GetGround()) && !(m_tPlayer.situation == SITUATION::Jump)
			&& !(m_tPlayer.situation == SITUATION::Weightless))
		{
			if (!m_tPlayer.Limit_Cheak)
			{
			
				s_fCheakTime += DT;
				if (s_fCheakTime >= 0.15f)
				{
					if (m_tPlayer.direction == DIRECTION::Right)
					{
						mAnimator->Play(L"Right_Fall", true);
						m_tPlayer.situation = SITUATION::Gravity;
						m_tPlayer.direction = DIRECTION::Right;
					}
					if (m_tPlayer.direction == DIRECTION::Left)
					{
						mAnimator->Play(L"Left_Fall", true);
						m_tPlayer.situation = SITUATION::Gravity;
						m_tPlayer.direction = DIRECTION::Left;
					}
					s_fCheakTime = 0.f;
				}
			}
		}

		//낙하 하는 애니메이션 중 땅에 도달하면 나오는 애니메이션
		if (GetComponent<Rigidbody>()->GetGround() && m_tPlayer.situation == SITUATION::Gravity)
		{
			if (!m_tPlayer.Limit_Cheak)
			{
				m_pSFloating->Stop(true);
				m_pSLand->Play(false);
				if (m_tPlayer.direction == DIRECTION::Right)
				{					
					m_tPlayer.situation = SITUATION::End;
					m_tPlayer.direction = DIRECTION::Right;
				}
				if (m_tPlayer.direction == DIRECTION::Left)
				{
					m_tPlayer.situation = SITUATION::End;
					m_tPlayer.direction = DIRECTION::Left;
				}
				if (m_AttackAri)
					m_AttackAri = false;
				m_DashAir = false;
			}

		}


		if (KEY_DOWN(eKeyCode::Z))
		{
			if (!m_tPlayer.Limit_Cheak &&
				!(m_tPlayer.situation == SITUATION::Attack))
			{
				m_pSAttack->Play(false);				
					if (m_tPlayer.attack == Attack::Up && GetComponent<Rigidbody>()->GetGround())
					{
						mAnimator->Play(L"Player_Up_Attack", false);
						mAttackAnimator->Play(L"Up_AttackEffect", false);
						Player_Attack* Attack = ya::object::Instantiate<Player_Attack>(eColliderLayer::Player_Projecttile);
						Attack->SetTarget(this);
						Attack->SetCompass(COMPASS::Up);
						Attack->SetSkill(Player_Skill::Attack);
						m_tPlayer.situation = SITUATION::Up_Attack;
						m_tPlayer.direction = DIRECTION::Right;
					}
					else
					{
						if (m_tPlayer.direction == DIRECTION::Left && GetComponent<Rigidbody>()->GetGround())
						{
							mAttackAnimator->Play(L"Left_AttackEffect", false);
							mAnimator->Play(L"PlayerLeft_Attack", false);
							Player_Attack* Attack = ya::object::Instantiate<Player_Attack>(eColliderLayer::Player_Projecttile);
							Attack->SetTarget(this);
							Attack->SetCompass(COMPASS::Left);
							Attack->SetSkill(Player_Skill::Attack);
							m_tPlayer.situation = SITUATION::Attack;
							m_tPlayer.direction = DIRECTION::Left;
						}
						if (m_tPlayer.direction == DIRECTION::Right && GetComponent<Rigidbody>()->GetGround())
						{
							mAttackAnimator->Play(L"Right_AttackEffect", false);
							mAnimator->Play(L"PlayerRight_Attack", false);
							Player_Attack* Attack = ya::object::Instantiate<Player_Attack>(eColliderLayer::Player_Projecttile);
							Attack->SetTarget(this);
							Attack->SetCompass(COMPASS::Right);
							Attack->SetSkill(Player_Skill::Attack);
							m_tPlayer.situation = SITUATION::Attack;
							m_tPlayer.direction = DIRECTION::Right;
						}
					}
								
				//공중 에서 기본 공격 할때
				/*if (m_tPlayer.situation == SITUATION::Jump || m_tPlayer.situation == SITUATION::Weightless
					|| m_tPlayer.situation == SITUATION::Gravity)*/
				if (!GetComponent<Rigidbody>()->GetGround())
				{

					if ((m_tPlayer.attack == Attack::Down) || (m_tPlayer.attack == Attack::Up))
					{
						if (m_tPlayer.attack == Attack::Down && !(m_AttackAri))
						{
							mAnimator->Play(L"Player_Down_Attack", false);
							mAttackAnimator->Play(L"Down_AttackEffect", false);
							Player_Attack* Attack = ya::object::Instantiate<Player_Attack>(eColliderLayer::Player_Projecttile);
							Attack->SetTarget(this);
							Attack->SetCompass(COMPASS::Down);
							Attack->SetSkill(Player_Skill::Attack);
							m_AttackAri = true;
							m_tPlayer.situation = SITUATION::Down_Attack;
							m_tPlayer.direction = DIRECTION::Left;
						}
						if (m_tPlayer.attack == Attack::Up && !(m_AttackAri))
						{
							mAnimator->Play(L"Player_Up_Attack", false);
							mAttackAnimator->Play(L"Up_AttackEffect", false);
							Player_Attack* Attack = ya::object::Instantiate<Player_Attack>(eColliderLayer::Player_Projecttile);
							Attack->SetTarget(this);
							Attack->SetCompass(COMPASS::Up);	
							Attack->SetSkill(Player_Skill::Attack);
							m_AttackAri = true;
							m_tPlayer.situation = SITUATION::Up_Attack;
							m_tPlayer.direction = DIRECTION::Right;
						}
					}
					else
					{
						if (m_tPlayer.direction == DIRECTION::Left && (m_tPlayer.attack == Attack::None))
						{
							mAttackAnimator->Play(L"Left_AttackEffect", false);
							mAnimator->Play(L"PlayerLeft_Attack", false);
							Player_Attack* Attack = ya::object::Instantiate<Player_Attack>(eColliderLayer::Player_Projecttile);
							Attack->SetTarget(this);
							Attack->SetCompass(COMPASS::Left);
							Attack->SetSkill(Player_Skill::Attack);
							m_tPlayer.situation = SITUATION::Attack;
							m_tPlayer.direction = DIRECTION::Left;
						}
						if (m_tPlayer.direction == DIRECTION::Right && (m_tPlayer.attack == Attack::None))
						{
							mAttackAnimator->Play(L"Right_AttackEffect", false);
							mAnimator->Play(L"PlayerRight_Attack", false);
							Player_Attack* Attack = ya::object::Instantiate<Player_Attack>(eColliderLayer::Player_Projecttile);
							Attack->SetTarget(this);
							Attack->SetCompass(COMPASS::Right);
							Attack->SetSkill(Player_Skill::Attack);
							m_tPlayer.situation = SITUATION::Attack;
							m_tPlayer.direction = DIRECTION::Right;
						}
					}		
				}
			}


		}
	
		if (m_bPlayerHit)
		{						
			
			if (m_tPlayer.situation == SITUATION::Modal)
			{
				s_fCheakTime += DT;
				if (s_fCheakTime >= 0.3f)
				{
					mbAniHit = true;
					m_tPlayer.situation = SITUATION::Hit;
					s_fCheakTime = 0.f;
					if(m_bHitAir)
					{
						Rigidbody* rigidbody = GetComponent<Rigidbody>();
						rigidbody->SetGround(false);
						m_bHitAir = false;
					}
				}
			}

			if (m_tPlayer.situation == SITUATION::Hit)
			{
				s_fCheakTime += DT;
				if (s_fCheakTime >= 0.9f)
				{
					m_tPlayer.situation = SITUATION::None;

					m_bPlayerHit = false;
					s_fCheakTime = 0.f;
				}
			}
		}
		
		if (KEY_DOWN(eKeyCode::V))
		{
			



		}		
		if (KEY_PRESSE(eKeyCode::V))
		{
			

			if(mMp >= 2)
			{
				if (m_bHp == false)
				{
					s_fSkillTimer += DT;
					if (s_fSkillTimer >= 0.7 && s_fSkillTimer <= 0.75)
					{
						m_pSHeal->Play(false);
						mAttackAnimator->Play(L"Player_Hp_Effect1", true);
						if (m_tPlayer.direction == DIRECTION::Left)
							mAnimator->Play(L"Player_Left_Hp1", true);
						else if (m_tPlayer.direction == DIRECTION::Right)
							mAnimator->Play(L"Player_Right_Hp1", true);
					}
					if (s_fSkillTimer >= 2.85 && s_fSkillTimer <= 2.90)
					{
						mAttackAnimator->Play(L"Player_Hp_Effect2", false);
					}
					if (s_fSkillTimer >= 3.0)
					{
						if (m_tPlayer.direction == DIRECTION::Left)
							mAnimator->Play(L"Left_Standing", true);
						else if (m_tPlayer.direction == DIRECTION::Right)
							mAnimator->Play(L"Right_Standing", true);

						m_pSHeal->Stop(false);
						m_pSHeal1->Play(false);
						mAttackAnimator->Play(L"Player_Cure_Effect", false);
						//Hp 회복 hp 회복은 5보다 낮을때만 회복 full hp 일때는 mp만 깍임.
						m_bHp = true;
						if (mHp < 5)
							++mHp;
							--mMp;
							--mMp;
						s_fSkillTimer = 0.f;
					}
				}
			}

		}

		if (KEY_UP(eKeyCode::V))
		{

			if (s_fSkillTimer >= 0.001 && s_fSkillTimer <= 0.7)
			{
				if (m_tPlayer.direction == DIRECTION::Right)
				{
					mAnimator->Play(L"Player_Right_Magic_Attack", false);
					mAttackAnimator->Play(L"Player_Right_Magic_Effect", false);
					Player_Attack* Attack = ya::object::Instantiate<Player_Attack>(eColliderLayer::Player_Projecttile);
					Attack->SetTarget(this);
					Attack->SetCompass(COMPASS::Right);
					Attack->SetSkill(Player_Skill::Magic);
				}
				else if (m_tPlayer.direction == DIRECTION::Left)
				{
					mAnimator->Play(L"Player_Left_Magic_Attack", false);
					mAttackAnimator->Play(L"Player_Left_Magic_Effect", false);
					Player_Attack* Attack = ya::object::Instantiate<Player_Attack>(eColliderLayer::Player_Projecttile);
					Attack->SetTarget(this);
					Attack->SetCompass(COMPASS::Left);
					Attack->SetSkill(Player_Skill::Magic);
				}

				m_pSMagic->Play(false);
				m_bMagic = true;	
				--mMp;
				--mMp;
				//원거리 공격
			}

			else if(s_fSkillTimer > 0.7 && s_fSkillTimer < 3.0)
			{
				m_pSHeal->Stop(false);
				if (m_tPlayer.direction == DIRECTION::Left)
					mAnimator->Play(L"Left_Standing", true);
				else if (m_tPlayer.direction == DIRECTION::Right)
					mAnimator->Play(L"Right_Standing", true);

				mAttackAnimator->Play(L"Player_Hp_Effect3", false);
			}

			m_bHp = false;
			s_fSkillTimer = 0.f;
		}

		if (KEY_DOWN(eKeyCode::N_3))
		{
			mMp =+ 1000;
		}

		if (m_bMagic)
		{
			s_fSkillTimer += DT;

			if(s_fSkillTimer >= 0.7)
			{
				if (m_tPlayer.direction == DIRECTION::Left)
					mAnimator->Play(L"Left_Standing", true);
				else if (m_tPlayer.direction == DIRECTION::Right)
					mAnimator->Play(L"Right_Standing", true);

				mAttackAnimator->Play(L"Player_Hp_Effect3", false);
				m_bMagic = false;
				s_fSkillTimer = 0.f;
			}
			
		}



	}


	void Player::Render(HDC hdc)
	{
		//HBRUSH blueBrush = CreateSolidBrush(RGB(153, 204, 255));
		//Brush brush(hdc, blueBrush);

		//HPEN redPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		//Pen pen(hdc, redPen);

		//GDI+ 이미지 
		//if(m_tPlayer.Direction == STATE::Right_Dash)
		//{
		//	Graphics graphic(hdc);
		//	std::wstring path = L"..\\Resources\\Image\\Dasheffect.png";
		//	Image* pImage = new Image(path.c_str());

		//	Vector2 pngImg = Camera::CalculatePos(GetPos());
		//	//X, Y 위치에  이미지의 X, Y, CX, CY 만큼 그려준다
		//	graphic.DrawImage(pImage, (int)pngImg.x - 650, (int)pngImg.y - 225, 0, 0, 500, 320, UnitPixel);
		//}


		//Player 피격시 깜빡깜빡 하는 이펙트 구현
		static float fRatio = 1.f;	//비율을 1부터 해줘서 캐릭터가 평소에는 보이게 해준다
		static float Dir = -1.f;

		if (mbAniHit)
		{
			m_time += DT;			//깜빡임 지속 시간 타이머
			fRatio += DT * Dir * 20.f;

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
			if (m_time > 0.9f)		//0.9초 후 다시 원래 상태로 돌림
			{
				mbAniHit = false;
				m_time = 0;
				fRatio = 1.f;
				Dir = -1.f;
			}
		}

		m_iAniHit = (int)((255) * (fRatio));

		if (KEY_DOWN(eKeyCode::N_1))
		{

			Collider* ColliderOnOff = new Collider();
			ColliderOnOff->SetScale(Vector2::Zero);
			if (ColliderOnOff->GetCheakRen())
			{
				ColliderOnOff->SetCheakRen(false);
			}
			else
			{
				ColliderOnOff->SetCheakRen(true);
			}
			AddComponent(ColliderOnOff);
		}

		if (KEY_DOWN(eKeyCode::N_4))
		{
			Camera::SetFalse(false);
			Camera::SetHornet(false);

			SetPos(m_vFixPos);
		}

		GameObject::Render(hdc);
	}

	void Player::OnCollisionEnter(Collider* other)
	{
		
		if (m_bInvincibility == false)
		{

			if (dynamic_cast<Husk*>(other->GetOwner()))
			{
				if (mHp <= 1)
				{
					m_tPlayerAction = Player_Action::Death;
				}
				m_pSHit->Play(false);
				--this->mHp;
				Rigidbody* rigidbody = GetComponent<Rigidbody>();
				rigidbody->SetVelocity(Vector2::Zero);
				if (!(rigidbody->GetGround()))
				{
					rigidbody->SetGround(true);
					m_bHitAir = true;
				}
				m_tPlayer.Time_Limit = 0.9f;
				m_tPlayer.Limit_Cheak = true;
				mAnimator->Play(L"Player_Damage", false);
				mAttackAnimator->Play(L"Player_Damage_Effect", false);
				m_tPlayer.situation = SITUATION::Modal;
				m_bPlayerHit = true;


			}
			if (dynamic_cast<Baldur*>(other->GetOwner()))
			{
				if (mHp <= 1)
				{
					m_tPlayerAction = Player_Action::Death;
				}
				m_pSHit->Play(false);
				--this->mHp;
				Rigidbody* rigidbody = GetComponent<Rigidbody>();
				rigidbody->SetVelocity(Vector2::Zero);
				if (!(rigidbody->GetGround()))
				{
					rigidbody->SetGround(true);
					m_bHitAir = true;
				}
				m_tPlayer.Time_Limit = 0.9f;
				m_tPlayer.Limit_Cheak = true;
				mAnimator->Play(L"Player_Damage", false);
				mAttackAnimator->Play(L"Player_Damage_Effect", false);
				m_tPlayer.situation = SITUATION::Modal;
				m_bPlayerHit = true;
			}
			if (dynamic_cast<Hornet*>(other->GetOwner()))
			{
				if (mHp <= 1)
				{
					m_tPlayerAction = Player_Action::Death;
				}
				m_pSHit->Play(false);
				--this->mHp;
				Rigidbody* rigidbody = GetComponent<Rigidbody>();
				rigidbody->SetVelocity(Vector2::Zero);
				if (!(rigidbody->GetGround()))
				{
					rigidbody->SetGround(true);
					m_bHitAir = true;
				}
				m_tPlayer.Time_Limit = 0.9f;
				m_tPlayer.Limit_Cheak = true;
				mAnimator->Play(L"Player_Damage", false);
				mAttackAnimator->Play(L"Player_Damage_Effect", false);
				m_tPlayer.situation = SITUATION::Modal;
				m_bPlayerHit = true;
			}
			if (dynamic_cast<Hornet_Attack*>(other->GetOwner()))
			{
				if (mHp <= 1)
				{
					m_tPlayerAction = Player_Action::Death;
				}
				m_pSHit->Play(false);
				--this->mHp;
				Rigidbody* rigidbody = GetComponent<Rigidbody>();
				rigidbody->SetVelocity(Vector2::Zero);
				if (!(rigidbody->GetGround()))
				{
					rigidbody->SetGround(true);
					m_bHitAir = true;
				}
				m_tPlayer.Time_Limit = 0.9f;
				m_tPlayer.Limit_Cheak = true;
				mAnimator->Play(L"Player_Damage", false);
				mAttackAnimator->Play(L"Player_Damage_Effect", false);
				m_tPlayer.situation = SITUATION::Modal;
				m_bPlayerHit = true;
			}

			if (dynamic_cast<False_Attack*>(other->GetOwner()))
			{
				if (mHp <= 1)
				{
					m_tPlayerAction = Player_Action::Death;
				}
				m_pSHit->Play(false);
				--this->mHp;
				Rigidbody* rigidbody = GetComponent<Rigidbody>();
				rigidbody->SetVelocity(Vector2::Zero);
				if (!(rigidbody->GetGround()))
				{
					rigidbody->SetGround(true);
					m_bHitAir = true;
				}
				m_tPlayer.Time_Limit = 0.9f;
				m_tPlayer.Limit_Cheak = true;
				mAnimator->Play(L"Player_Damage", false);
				mAttackAnimator->Play(L"Player_Damage_Effect", false);
				m_tPlayer.situation = SITUATION::Modal;
				m_bPlayerHit = true;
			}

			if (dynamic_cast<False_Knight*>(other->GetOwner()))
			{
				if (mHp <= 1)
				{
					m_tPlayerAction = Player_Action::Death;
				}
				m_pSHit->Play(false);
				--this->mHp;
				Rigidbody* rigidbody = GetComponent<Rigidbody>();
				rigidbody->SetVelocity(Vector2::Zero);
				if (!(rigidbody->GetGround()))
				{
					rigidbody->SetGround(true);
					m_bHitAir = true;
				}
				m_tPlayer.Time_Limit = 0.9f;
				m_tPlayer.Limit_Cheak = true;
				mAnimator->Play(L"Player_Damage", false);
				mAttackAnimator->Play(L"Player_Damage_Effect", false);
				m_tPlayer.situation = SITUATION::Modal;				
				m_bPlayerHit = true;
			}


		}


		
		
		//나중에 하기?
		//Scene* playScene = SceneManager::GetPlayScene();
		//std::vector<GameObject*> monster = playScene->GetGameObjects(eColliderLayer::Monster);
		//for (size_t i = 0; monster.size();  ++i)
		//{
		//	if (monster[i]== other->GetOwner())
		//	{
		//		--this->mHp;
		//			break;
		//	}
		//}
		


	}

	void Player::OnCollisionStay(Collider* other)
	{

	}

	void Player::OnCollisionExit(Collider* other)
	{

	}

	void Player::WalkComplete()
	{
		Missile* missile = new Missile();

		Scene* playScene = SceneManager::GetPlayScene();
		playScene->AddGameObject(missile, eColliderLayer::Player_Projecttile);

		Vector2 playerPos = GetPos();
		Vector2 playerScale = GetScale() / 2.0f;
		Vector2 missileScale = missile->GetScale();

		missile->SetPos((playerPos + playerScale) - (missileScale / 2.0f));
	}




	void Player::TextureInitialize()
	{
		if (nullptr == m_pStateImg)
		{
			m_pStateImg = Resources::Load<Texture>(L"Player_State", L"..\\Resources\\Image\\Player_State.bmp");
		}	
		if (nullptr == m_pPlayerAttackeffect)
		{
			m_pPlayerAttackeffect = Resources::Load<Texture>(L"AttackEffect", L"..\\Resources\\Image\\AttackEffect.bmp");
		}
		if (nullptr == m_pPlayerAttack_Up_Down_effect)
		{
			m_pPlayerAttack_Up_Down_effect = Resources::Load<Texture>(L"AttackEffect_Up_Down", L"..\\Resources\\Image\\AttackEffect_Up_Down.bmp");
		}
		if (nullptr == m_pPlayerDashEffect)
		{
			m_pPlayerDashEffect = Resources::Load<Texture>(L"Dash_Effect", L"..\\Resources\\Image\\Dash_Effect.bmp");
		}
		if (nullptr == m_pPlayerHit)
		{
			m_pPlayerHit = Resources::Load<Texture>(L"Player_Hit", L"..\\Resources\\Image\\Player_Hit.bmp");
		}		
		

	}
	void Player::AnimationInitialize()
	{

		//기본 왼쪽 오른쪽 자세
		mAnimator->CreateAnimation(L"Left_Standing", m_pStateImg
			, Vector2(0.0f, 732.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 8, 0.3f);
		mAnimator->CreateAnimation(L"Right_Standing", m_pStateImg
			, Vector2(0.0f, 854.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 8, 0.3f);

		//기본 걷는 모션
		mAnimator->CreateAnimation(L"Left_Walk", m_pStateImg
			, Vector2(0.0f, 488.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 8, 0.1f);
		mAnimator->CreateAnimation(L"Right_Walk", m_pStateImg
			, Vector2(0.0f, 610.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 8, 0.1f);

		//기본 위 방향 
		mAnimator->CreateAnimation(L"Left_Up_down", m_pStateImg
			, Vector2(0.0f, 244.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		mAnimator->CreateAnimation(L"Left_Up_presse", m_pStateImg
			, Vector2(640.0f, 244.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 1, 0.1f);
		mAnimator->CreateAnimation(L"Left_Up_up", m_pStateImg
			, Vector2(768.0f, 244.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);

		mAnimator->CreateAnimation(L"Right_Up_down", m_pStateImg
			, Vector2(0.0f, 366.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		mAnimator->CreateAnimation(L"Right_Up_presse", m_pStateImg
			, Vector2(640.0f, 366.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 1, 0.1f);
		mAnimator->CreateAnimation(L"Right_Up_up", m_pStateImg
			, Vector2(768.0f, 366.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);

		//기본 아래 방향
		mAnimator->CreateAnimation(L"Left_Down_down", m_pStateImg
			, Vector2(0.0f, 0.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		mAnimator->CreateAnimation(L"Left_Down_presse", m_pStateImg
			, Vector2(640.0f, 0.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 1, 0.1f);
		mAnimator->CreateAnimation(L"Left_Down_up", m_pStateImg
			, Vector2(768.0f, 0.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);

		mAnimator->CreateAnimation(L"Right_Down_down", m_pStateImg
			, Vector2(0.0f, 122.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 5, 0.1f);
		mAnimator->CreateAnimation(L"Right_Down_presse", m_pStateImg
			, Vector2(640.0f, 122.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 1, 0.1f);
		mAnimator->CreateAnimation(L"Right_Down_up", m_pStateImg
			, Vector2(768.0f, 122.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);


		//대쉬 스킬 (대쉬 스킬 중에는 무적 판정 + 커맨드 입력 제한)
		mAnimator->CreateAnimation(L"PlayerLeft_Dash", m_pStateImg
			, Vector2(0.0f, 976.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 8, 0.06f);
		mAnimator->CreateAnimation(L"PlayerRight_Dash", m_pStateImg
			, Vector2(0.0f, 1098.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 8, 0.06f);

		//기본 공격 모션
		mAnimator->CreateAnimation(L"PlayerLeft_Attack", m_pStateImg
			, Vector2(0.0f, 1220.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 7, 0.04f);
		mAnimator->CreateAnimation(L"PlayerRight_Attack", m_pStateImg
			, Vector2(0.0f, 1342.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 7, 0.04f);

		mAnimator->CreateAnimation(L"Player_Up_Attack", m_pStateImg
			, Vector2(0.0f, 1708.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);
		mAnimator->CreateAnimation(L"Player_Down_Attack", m_pStateImg
			, Vector2(0.0f, 1830.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);
		//원거리 공격 모션
		mAnimator->CreateAnimation(L"Player_Left_Magic_Attack", m_pStateImg
			, Vector2(0.0f, 2074.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 7, 0.1f);
		mAnimator->CreateAnimation(L"Player_Right_Magic_Attack", m_pStateImg
			, Vector2(0.0f, 2196.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 7, 0.1f);


		//Hp 회복 모션 
		mAnimator->CreateAnimation(L"Player_Left_Hp1", m_pStateImg
			, Vector2(384.0f, 1708.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);
		mAnimator->CreateAnimation(L"Player_Right_Hp1", m_pStateImg
			, Vector2(384.0f, 1830.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);


		//일단 대기
		//mAnimator->CreateAnimation(L"Player_Left_Hp2", m_pStateImg
		//	, Vector2(512.0f, 1708.0f), Vector2(128.0f, 122.0f)
		//	, Vector2(0.0f, 0.0f), 3, 0.1f);
		//mAnimator->CreateAnimation(L"Player_Right_Hp2", m_pStateImg
		//	, Vector2(512.0f, 1830.0f), Vector2(128.0f, 122.0f)
		//	, Vector2(0.0f, 0.0f), 3, 0.1f);

		//점프 모션 -> 착지까지
		mAnimator->CreateAnimation(L"Right_Jump1", m_pStateImg
			, Vector2(0.0f, 1464.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);
		mAnimator->CreateAnimation(L"Right_Jump2", m_pStateImg
			, Vector2(384.0f, 1464.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 1, 0.1f);
		mAnimator->CreateAnimation(L"Right_Jump3", m_pStateImg
			, Vector2(512.0f, 1464.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);
		mAnimator->CreateAnimation(L"Right_Fall", m_pStateImg
			, Vector2(768.0f, 1464.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);
		mAnimator->CreateAnimation(L"Right_Landing", m_pStateImg
			, Vector2(1024.0f, 1464.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 2, 0.03f);

		mAnimator->CreateAnimation(L"Left_Jump1", m_pStateImg
			, Vector2(0.0f, 1586.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 3, 0.1f);
		mAnimator->CreateAnimation(L"Left_Jump2", m_pStateImg
			, Vector2(384, 1586.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 1, 0.1f);
		mAnimator->CreateAnimation(L"Left_Jump3", m_pStateImg
			, Vector2(512.0f, 1586.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);
		mAnimator->CreateAnimation(L"Left_Fall", m_pStateImg
			, Vector2(768.0f, 1586.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 2, 0.1f);
		mAnimator->CreateAnimation(L"Left_Landing", m_pStateImg
			, Vector2(1024.0f, 1586.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 2, 0.03f);

		//대쉬 이펙트
		mAttackAnimator->CreateAnimation(L"Left_DashEffect", m_pPlayerDashEffect
			, Vector2(0.0f, 0.0f), Vector2(503.0f, 334.0f)
			, Vector2(0.0f, 0.0f), 6, 0.08f);
		mAttackAnimator->CreateAnimation(L"Right_DashEffect", m_pPlayerDashEffect
			, Vector2(0.0f, 334.0f), Vector2(503.0f, 334.0f)
			, Vector2(0.0f, 0.0f), 6, 0.08f);


		//피격 애니메이션 + 이펙트 
		mAnimator->CreateAnimation(L"Player_Damage", m_pStateImg
			, Vector2(0.0f, 1952.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 6, 0.15f);
		mAttackAnimator->CreateAnimation(L"Player_Damage_Effect", m_pPlayerHit
			, Vector2(0.0f, 0.0f), Vector2(699.0f, 140.0f)
			, Vector2(0.0f, 0.0f), 4, 0.1f);

		//Hp 회복 Effect
		mAttackAnimator->CreateAnimation(L"Player_Hp_Effect1", m_pPlayerHit
			, Vector2(0.0f, 140.0f), Vector2(75.0f, 245.0f)
			, Vector2(0.0f, -50.0f), 4, 0.1f);
		mAttackAnimator->CreateAnimation(L"Player_Hp_Effect2", m_pPlayerHit
			, Vector2(300.0f, 140.0f), Vector2(75.0f, 245.0f)
			, Vector2(0.0f, -50.0f), 1, 0.1f);
		mAttackAnimator->CreateAnimation(L"Player_Hp_Effect3", m_pPlayerHit
			, Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f)
			, Vector2(0.0f, 0.0f), 1, 0.1f);

		mAttackAnimator->CreateAnimation(L"Player_Cure_Effect", m_pPlayerHit
			, Vector2(0.0f, 929.0f), Vector2(254.0f, 281.0f)
			, Vector2(0.0f, 0.0f), 6, 0.1f);

		//Magic Effect
		mAttackAnimator->CreateAnimation(L"Player_Left_Magic_Effect", m_pPlayerHit
			, Vector2(0.0f, 385.0f), Vector2(288.0f, 272.0f)
			, Vector2(-60.0f, 0.0f), 6, 0.1f);
		mAttackAnimator->CreateAnimation(L"Player_Right_Magic_Effect", m_pPlayerHit
			, Vector2(0.0f, 657.0f), Vector2(288.0f, 272.0f)
			, Vector2(60.0f, 0.0f), 6, 0.1f);


		//공격 이펙트
		//왼쪽 오른쪽
		mAttackAnimator->CreateAnimation(L"Left_AttackEffect", m_pPlayerAttackeffect
			, Vector2(0.0f, 0.0f), Vector2(260.0f, 188.0f)
			, Vector2(-40.0f, 0.0f), 5, 0.05f);
		mAttackAnimator->CreateAnimation(L"Right_AttackEffect", m_pPlayerAttackeffect
			, Vector2(0.0f, 188.0f), Vector2(260.0f, 188.0f)
			, Vector2(40.0f, 0.0f), 5, 0.05f);

		mAttackAnimator->CreateAnimation(L"Up_AttackEffect", m_pPlayerAttack_Up_Down_effect
			, Vector2(0.0f, 0.0f), Vector2(160.0f, 260.0f)
			, Vector2(20.0f, -50.0f), 5, 0.04f);
		mAttackAnimator->CreateAnimation(L"Down_AttackEffect", m_pPlayerAttack_Up_Down_effect
			, Vector2(0.0f, 260.0f), Vector2(160.0f, 260.0f)
			, Vector2(-20.0f, 50.0f), 5, 0.04f);

		//들어가는 모션
		mAnimator->CreateAnimation(L"Player_In", m_pStateImg
			, Vector2(0.0f, 2440.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 10, 0.1f);

		//Save 모션
		mAnimator->CreateAnimation(L"Player_Sit_Up1", m_pStateImg
			, Vector2(0.0f, 2562.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 6, 0.1f);


		//Death
		mAnimator->CreateAnimation(L"Player_Death", m_pStateImg
			, Vector2(0.0f, 2318.0f), Vector2(128.0f, 122.0f)
			, Vector2(0.0f, 0.0f), 7, 0.15f);

		
	}
	void Player::SoundInitialize()
	{
		if(nullptr == m_pSAttack)
		m_pSAttack = Resources::Load<Sound>(L"Player_sword_1", L"..\\Resources\\Sound\\Player_sword_1.wav");
		if(nullptr == m_pSMagic)
		m_pSMagic = Resources::Load<Sound>(L"Player_fireball", L"..\\Resources\\Sound\\Player_fireball.wav");
		if(nullptr == m_pSDash)
		m_pSDash = Resources::Load<Sound>(L"Player_Dash_S", L"..\\Resources\\Sound\\Player_Dash_S.wav");
		if(nullptr == m_pSWalk)
		m_pSWalk = Resources::Load<Sound>(L"Player_Walk_S", L"..\\Resources\\Sound\\Player_Walk_S.wav");
		if(nullptr == m_pSJump)
		m_pSJump = Resources::Load<Sound>(L"Player_Jump_S", L"..\\Resources\\Sound\\Player_Jump_S.wav");
		if(nullptr == m_pSFloating)
		m_pSFloating = Resources::Load<Sound>(L"Player_Floating_S", L"..\\Resources\\Sound\\Player_Floating_S.wav");
		if(nullptr == m_pSHeal)
		m_pSHeal = Resources::Load<Sound>(L"Player_Heal_S", L"..\\Resources\\Sound\\Player_Heal_S.wav");
		if(nullptr == m_pSHeal1)
		m_pSHeal1 = Resources::Load<Sound>(L"Player_Heal_S1", L"..\\Resources\\Sound\\Player_Heal_S1.wav");
		if(nullptr == m_pSLand)
		m_pSLand = Resources::Load<Sound>(L"Player_Land_S", L"..\\Resources\\Sound\\Player_Land_S.wav");
		if(nullptr == m_pSHit)
		m_pSHit = Resources::Load<Sound>(L"Player_Hit_S", L"..\\Resources\\Sound\\Player_Hit_S.wav");
		if(nullptr == m_pDeath)
		m_pDeath = Resources::Load<Sound>(L"Player_Death_S", L"..\\Resources\\Sound\\Player_Death_S.wav");

		



	}
	bool Player::Timer(float time)
	{
		
		while(true)
		{
			s_fTimer += DT;
			if(s_fTimer > time)		
			{
				s_fTimer = 0.f;
				break;
			}
		}	
		return true;
	}
}
