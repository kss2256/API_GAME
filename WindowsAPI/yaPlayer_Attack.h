#pragma once
#include "yaGameObject.h"

namespace ya
{

	enum class Player_Skill
	{
		Attack,
		Magic,
		Magic_Walk,
		Magic_Hit,
		Magic_Death,

	};

	enum class COMPASS
	{
		None,
		Up,
		Down,
		Left,
		Right,
	};
	class Texture;
	class Animator;
	class Collider;
	class Player_Attack
		: public GameObject
	{
	private:

		COMPASS			m_eCompass;
		Player_Skill	m_eSkill;
		Collider*		m_pCollider;
		Animator*		m_pAttack;
		Texture*		m_pTexture;
		static float	s_fTimer;
		GameObject*		m_pPlayer;
		Vector2			m_vMovePos;

		bool			m_bMagicHit;

	public:

		virtual void Tick() override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;

		void SetTarget(GameObject* target) { m_pPlayer = target; }
		GameObject* GetTarget() { return m_pPlayer; }

		COMPASS GetCompass() { return m_eCompass; }
		void SetCompass(COMPASS direction) { m_eCompass = direction; }
		Player_Skill GetSkill() { return  m_eSkill; }
		void SetSkill(Player_Skill skill) { m_eSkill = skill; }
		bool GetHit() { return m_bMagicHit; }
		void SetHit(bool hit) { m_bMagicHit = hit; }

	public:
		Player_Attack();
		~Player_Attack();


	};



}
