#pragma once
#include "yaGameObject.h"
namespace ya
{
	enum class FALSE_SKILL	//�̰� �������� ���� �� ���� ���� None���¿��� ���� ����
	{
		None,				//None ���¿��� �ǰ� ���ϸ� �ڷ� �и��� ������ End�� ���� �ٽ� Ȯ��
		Stun,				//
		Hammer_Attack,		//�⺻ ��ġ ����	
		Hammer_Earthquake,	//�������� ��ġ ���� (�ϴÿ��� ��ü ������)
		Hammer_Jump,		//�����ؼ� ��ġ�� ������� ����
		Hammer_Shock_Wave,	//�⺻ ��ġ ���Ƽ� ���� (����� ����)		
		Shoulder_Attack,	//�޷���� ����	(�̶��� ���� ���� �ֳ�?)
		Death,
		End,				//��ų ��� �� �ϰ� End ���·� �ΰ�, �ǰ� ���� Ȯ�� �Ŀ�
	};						//�ǰ� ���°� �ƴϸ� None ���·� �ٲ��ְ� End�����϶� �ǰ� ���¸� None��

	enum class MONSTER_ACTION	//�ǰ� �ÿ� ���� ���� ��Ȳ Ȯ�� �Ϸ��� ����.
	{
		None,				//
		Attack,				//���� ���� �̰�
		Walk,				//���� �ϱ����� ������ ������ �̰�
		End,
	};


	enum class PROGRESS		//��ų �۾� �Ҷ� �ϴ� ��� �Ұ�
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


		FALSE_AI		m_tMonster;		//���� Engine
		FALSE_AI		m_tHitSaveAI;	//���� �ǰݽ� �ൿ �����
		Texture*		m_pFalseImg;	//Img �ε�

		Animator*		m_pFalseAttack;
		Animator*		m_pFalseEffect;
		Collider*		m_pCollider;
		False_Attack*	m_Attack;
		GameObject*		m_pPlayer;		//�÷��̾� ��ġ Ȯ��

		Vector2         m_vTargetPos;  //�÷��̾� ��ġ ����
		Vector2         m_vFixPos;     //���� ��ȯ ��ġ ����(������)
		Vector2         m_vMovePos;    //���� ��ġ 
		Vector2         m_vAttackPos;  //���� ���� ����

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

		int             m_Hp;			//���� Hp
		int				m_StunHp;
		static int      s_iRandom;      //���� �ൿ 
		static int      s_Attack_Repeat;//�ظ� ���� ���� �ݺ� ȸ�� 
		static float    s_fTimer;
		static bool     s_bStart;
		bool            m_bTar;			//�÷��̾� ������ ���� Tick Ȱ��ȭ
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
		//�̹��� �ִϸ��̼� ���ε�
		void AnimationInitialize();
		//���� ���ε�
		void SoundInitialize();
		//Player ��ġ�� ������ ���� �������� Ȯ��
		bool TargetPlayer();

		//Monster Attack ����
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


		//�Լ�����
		void Earthquak();
		void Attack();

	public:
		False_Knight();
		~False_Knight();


	};

}