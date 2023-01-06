#pragma once
#include "yaGameObject.h"

namespace ya
{
	class Sound;
	class Collider;
	class Texture;
	class Animator;
	class Baldur
		: public GameObject
	{

	private:
		int             m_Hp;
		int				m_iAniHit;

		static float	s_fTimer;      //���� �ൿ ������
		static float	m_time;

		Command         m_tMonster;      //���� �ൿ ����

		Vector2         m_vTargetPos;   //�÷��̾� ��ġ ����
		Vector2         m_vFixPos;      //���� ��ȯ ��ġ ����(������)
		Vector2         m_vMovePos;     //���� ��ġ 
		Vector2         m_vAttackPos;   //���� ���� ����

		Texture*		m_pBaldurImg;     //���� �̹���
		Animator*		m_pBaldurAni;     //���� �ִϸ��̼�
		GameObject*		m_pPlayer;      //�÷��̾� ��ġ
		Collider*		m_pMonsterSize;	//���� �ݶ��̴� ũ�� ������

		Sound*			m_Sound;

		bool            m_bTarget;      //���� ��������
		bool            m_bTar;			//�÷��̾ �ִ��� Ȯ��
		bool			mbAniHit;
		bool			m_bFix;

		bool	m_bLeftWalkCheak;
		bool	m_bRightWalkCheak;

	public:

		virtual void Tick() override;
		virtual void Render(HDC hdc) override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;

		void SetTarget(GameObject* target) { m_pPlayer = target; }
		GameObject* GetTarget() { return m_pPlayer; }

		void SetBTar(bool target) { m_bTar = target; }
		bool GetBTar() { return m_bTar; }

		void SetHp(int hp) { m_Hp = hp; }
		int GetHp() { return m_Hp; }

		int GetAniHit() { return m_iAniHit; }
		void SetAniHit(int hit) { m_iAniHit = hit; }

		void SetLeftWalk(bool walk) { m_bLeftWalkCheak = walk; }
		bool GetLeftWalk() { return m_bLeftWalkCheak; }

		void SetRightWalk(bool walk) { m_bRightWalkCheak = walk; }
		bool GetRightWalk() { return m_bRightWalkCheak; }

		void SetWalk(Command walk) { m_tMonster = walk; }
		Command GetWalk() { return m_tMonster; }

	private:
		//�̹��� �ִϸ��̼� ���ε�
		void AnimationInitialize();
		//���� ���� ������ ���� ��ȯ ���ִ� �Լ�
		void Baldur_Left_Turn();
		void Baldur_Right_Turn();

	public:
		Baldur();
		~Baldur();



	};

}
