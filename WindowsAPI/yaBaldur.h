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

		static float	s_fTimer;      //몬스터 행동 딜레이
		static float	m_time;

		Command         m_tMonster;      //몬스터 행동 지시

		Vector2         m_vTargetPos;   //플레이어 위치 저장
		Vector2         m_vFixPos;      //몬스터 소환 위치 저장(기준점)
		Vector2         m_vMovePos;     //몬스터 위치 
		Vector2         m_vAttackPos;   //몬스터 공격 범위

		Texture*		m_pBaldurImg;     //몬스터 이미지
		Animator*		m_pBaldurAni;     //몬스터 애니메이션
		GameObject*		m_pPlayer;      //플레이어 위치
		Collider*		m_pMonsterSize;	//몬스터 콜라이더 크기 조절용

		Sound*			m_Sound;

		bool            m_bTarget;      //몬스터 추적상태
		bool            m_bTar;			//플레이어가 있는지 확인
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
		//이미지 애니메이션 업로드
		void AnimationInitialize();
		//몬스터 왼쪽 오른쪽 방향 전환 해주는 함수
		void Baldur_Left_Turn();
		void Baldur_Right_Turn();

	public:
		Baldur();
		~Baldur();



	};

}
