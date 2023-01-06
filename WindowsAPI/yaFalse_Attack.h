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

		Vector2				m_vTargetPos;  //��ġ ����
		Vector2				m_vFixPos;     //���� ��ġ ����(��������)
		Vector2				m_vMovePos;    //���� ��ġ 
		Vector2				m_vAttackPos;  //���� ����

		Texture*			m_pImg;
		
		False_Skill			m_eSkill;
		Attack_Direction	n_sDirection;
		Animator*			m_pAniAttack;

		Collider*			m_pMonsterSize;	//�ݶ��̴� ũ�� ������
		GameObject*			m_pFalse;		//False_Knight ��ġ

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

