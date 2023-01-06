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

		Vector2         m_vTargetPos;  //ȣ�� ��ġ ����
		Vector2         m_vFixPos;     //���� ��ġ ����(��������)
		Vector2         m_vMovePos;    //���� ��ġ 
		Vector2         m_vAttackPos;  //���� ����

		Texture*		m_pImg;
		Animator*		m_pAniAttack;
		Animator*		m_pAniEffect;

		Collider*		m_pMonsterSize;	//�ݶ��̴� ũ�� ������
		GameObject*		m_pHornet;      //ȣ�� ��ġ

		Attack_Type		m_eAttack;		//���� ����
		Tarp_Type		m_eTrap;		//Ʈ�� ����

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


