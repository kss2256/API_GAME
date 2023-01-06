#pragma once
#include "yaGameObject.h"

enum class Attack_Type
{
	None,
	Right_Spear,
	Left_Spear,
	Right_Spear_Effect,
	Left_Spear_Effect,
	Trap,
	End,
};

enum class Tarp_Type
{
	None,
	Thread,
	Anticipate,
	Spike,
	Spike_Waiting,
	Bard_Loose,
	Bard_Break,
};

namespace ya
{
	class Texture;
	class Animator;
	class Texture;
	class Collider;
	class Hornet_Attack
		: public GameObject
	{

	private:

		Vector2         m_vTargetPos;  //호넷 위치 저장
		Vector2         m_vFixPos;     //공격 위치 저장(시작지점)
		Vector2         m_vMovePos;    //공격 위치 
		Vector2         m_vAttackPos;  //공격 범위

		Texture*		m_pImg;
		Animator*		m_pAniAttack;
		Animator*		m_pAniEffect;

		Collider*		m_pMonsterSize;	//콜라이더 크기 조절용
		GameObject*		m_pHornet;      //호넷 위치

		Attack_Type		m_eAttack;		//공격 종류
		Tarp_Type		m_eTrap;		//트랩 종류

		static float    s_fTimer;

		bool			m_bTrapBreak;

	public:

		virtual void Tick() override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;


		void SetTarget(GameObject* target) { m_pHornet = target; }
		GameObject* GetTarget() { return m_pHornet; }

		void SetAttack_Type(Attack_Type type) { m_eAttack = type; }
		Attack_Type GetAyyack_Type() { return m_eAttack; }

		void SetTrap_Type(Tarp_Type type) { m_eTrap = type; }
		Tarp_Type GetTrap_Type() { return m_eTrap; }

	private:
		void AnimationInitialize();

	public:
		Hornet_Attack();
		~Hornet_Attack();

	};

}


