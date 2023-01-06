#pragma once
#include "yaGameObject.h"
namespace ya
{
	enum class FALSE_SKILL	//이걸 랜덤으로 돌려 서 진행 예정 None상태에서 랜덤 시작
	{
		None,				//None 상태에서 피격 당하면 뒤로 밀리고 끝나면 End로 가서 다시 확인
		Stun,				//
		Hammer_Attack,		//기본 망치 공격	
		Hammer_Earthquake,	//양쪽으로 망치 공격 (하늘에서 물체 떨어짐)
		Hammer_Jump,		//점프해서 망치로 내려찍기 공격
		Hammer_Shock_Wave,	//기본 망치 기모아서 공격 (충격파 나감)		
		Shoulder_Attack,	//달려들며 공격	(이때는 무적 판정 주나?)
		Death,
		End,				//스킬 사용 다 하고 End 상태로 두고, 피격 상태 확인 후에
	};						//피격 상태가 아니면 None 상태로 바꿔주고 End상태일때 피격 상태면 None로

	enum class MONSTER_ACTION	//피격 시에 지금 현재 상황 확인 하려고 만듬.
	{
		None,				//
		Attack,				//공격 들어갈때 이거
		Walk,				//공격 하기전에 가까이 붙을때 이거
		End,
	};


	enum class PROGRESS		//스킬 작업 할때 일단 사용 할거
	{
		start,
		step_1,
		step_2,
		step_3,
		step_4,
		step_5,
		step_6,
		step_7,
		step_8,
		step_9,
		end,
	};

	struct FALSE_AI
	{

		PROGRESS		Progress;
		FALSE_SKILL		Skill;
		DIRECTION		Direction;
		MONSTER_ACTION	Action;
		bool			Hit;


		static FALSE_AI Zero;
	

	};
}
namespace ya
{
	class Sound;
	class False_Attack;
	class Animator;
	class Collider;
	class Texture;
	class False_Knight
		: public GameObject
	{
	private:


		FALSE_AI		m_tMonster;		//몬스터 Engine
		FALSE_AI		m_tHitSaveAI;	//몬스터 피격시 행동 저장용
		Texture*		m_pFalseImg;	//Img 로드

		Animator*		m_pFalseAttack;
		Animator*		m_pFalseEffect;
		Collider*		m_pCollider;
		False_Attack*	m_Attack;
		GameObject*		m_pPlayer;		//플레이어 위치 확인

		Vector2         m_vTargetPos;  //플레이어 위치 저장
		Vector2         m_vFixPos;     //몬스터 소환 위치 저장(기준점)
		Vector2         m_vMovePos;    //몬스터 위치 
		Vector2         m_vAttackPos;  //몬스터 공격 범위

		Sound*			m_sShock;
		Sound*			m_sSwing;
		Sound*			m_sStrike;
		Sound*			m_sJump;
		Sound*			m_sLand;
		Sound*			m_sJump_Attack;
		Sound*			m_sShoulder;
		Sound*			m_sEarthquake;
		Sound*			m_sEarthquake1;
		Sound*			m_sHit;
		Sound*			m_sStun;
		Sound*			m_sHead;

		int             m_Hp;			//몬스터 Hp
		int				m_StunHp;
		static int      s_iRandom;      //랜덤 행동 
		static int      s_Attack_Repeat;//해머 진동 공격 반복 회수 
		static float    s_fTimer;
		static bool     s_bStart;
		bool            m_bTar;			//플레이어 유무에 따른 Tick 활성화
		bool			m_StunCheak;	//
		bool			m_False_Death;
		
		bool	m_bLeftWalkCheak;
		bool	m_bRightWalkCheak;

	public:

		void SetTarget(GameObject* target) { m_pPlayer = target; }
		GameObject* GetTarget() { return m_pPlayer; }

		void SetBTar(bool target) { m_bTar = target; }
		bool GetBTar() { return m_bTar; }

		void SetHp(int hp) { m_Hp = hp; }
		int GetHp() { return m_Hp; }

		void SetLeftWalk(bool walk) { m_bLeftWalkCheak = walk; }
		bool GetLeftWalk() { return m_bLeftWalkCheak; }

		void SetRightWalk(bool walk) { m_bRightWalkCheak = walk; }
		bool GetRightWalk() { return m_bRightWalkCheak; }

		void SetWalk(FALSE_AI walk) { m_tMonster = walk; }
		FALSE_AI GetWalk() { return m_tMonster; }


		virtual void Tick() override;
		virtual void Render(HDC hdc) override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;

	private:
		//이미지 애니메이션 업로드
		void AnimationInitialize();
		//사운드 업로드
		void SoundInitialize();
		//Player 위치가 오른쪽 인지 왼쪽인지 확인
		bool TargetPlayer();

		//Monster Attack 모음
		void Hammer_Attack();
		void Hammer_Earthquake(int number);
		void Hammer_Shock_Wave();
		void Shoulder_Attack();
		void Jump_Attack();

		void Stun();
		void False_Death();
		void False_Turn();
		void False_Walk(float distance);
		void False_Jump(float distance);
		void False_BackJump(float distance);


		//함수모음
		void Earthquak();
		void Attack();

	public:
		False_Knight();
		~False_Knight();


	};

}