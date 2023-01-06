#pragma once
#include "yaGameObject.h"


enum class False_Skill
{
	None,
	Earthquak,
	Earthquak_Walk,
	Shock_Wave,
	Shock_Walk,
	Attack,
	Attack_Walk,
	Stun,
	End,
};

enum class Attack_Direction
{
	None,
	Left,
	Right,
	End,
};


namespace ya
{
	class Collider;
	class Animator;
	class Texture;
	class False_Attack
		: public GameObject
	{

	private:

		Vector2				m_vTargetPos;  //위치 저장
		Vector2				m_vFixPos;     //공격 위치 저장(시작지점)
		Vector2				m_vMovePos;    //공격 위치 
		Vector2				m_vAttackPos;  //공격 범위

		Texture*			m_pImg;
		
		False_Skill			m_eSkill;
		Attack_Direction	n_sDirection;
		Animator*			m_pAniAttack;

		Collider*			m_pMonsterSize;	//콜라이더 크기 조절용
		GameObject*			m_pFalse;		//False_Knight 위치

		static float		s_fTimer;

		bool				m_bShock;
		bool				m_bEarthquak;

	public:

		void SetTarget(GameObject* target) { m_pFalse = target; }
		GameObject* GetTarget() { return m_pFalse; }

		void SetSkill(False_Skill skill) { m_eSkill = skill; }
		False_Skill GetSkill() { return m_eSkill; }

		void SetDir(Attack_Direction dir) { n_sDirection = dir; }
		Attack_Direction GetDir() { return n_sDirection; }

	public:


		virtual void Tick() override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;

	private:
		void AnimationInitialize();

	public:
		False_Attack();
		~False_Attack();

	};


}

