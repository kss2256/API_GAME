#pragma once
#include "yaGameObject.h"

namespace ya
{




	enum class Player_Action
	{
		None,
		Death,
		Walk,
		End,
	};


	class Sound;
	class Animator;
	class Texture;
	class Player : public GameObject
	{
	public:
		Player();
		~Player();

		virtual void Tick() override;
		virtual void Render(HDC hdc) override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;

		void WalkComplete();

		void SetHp(int hp) { mHp = hp; }
		int GetHp() { return mHp; }
		void SetMp(int mp) { mMp = mp; }
		int GetMp() { return mMp; }
		
		int GetAniHit() { return m_iAniHit; }
		void SetAniHit(int hit) { m_iAniHit = hit; }

		Vector2 GetFixPos() { return m_vFixPos; }


		void SetResurrection(bool save) { m_bSave = save; }
		static bool GetResurrection() { return m_bSave; }

		State GetState() { return m_tPlayer; }
		void SetState(State state) { m_tPlayer = state; }

		void SetLeftWalk(bool walk) { m_bLeftWalkCheak = walk; }
		bool GetLeftWalk() { return m_bLeftWalkCheak; }

		void SetRightWalk(bool walk) { m_bRightWalkCheak = walk; }
		bool GetRightWalk() { return m_bRightWalkCheak; }

		void SetPlayerIn(bool walk) { m_bPlayerIn = walk; }
		bool GetPlayerIn() { return m_bPlayerIn; }

		void SetPlayerHeal(bool walk) { m_bHealSave = walk; }
		bool GetPlayerHeal() { return m_bHealSave; }

	private:
		
		//이미지 리소스 업로드
		void TextureInitialize();
		//이미지 애니메이션 업로드
		void AnimationInitialize();

		void SoundInitialize();

		bool Timer(float time);

	private:

		float			mSpeed;
		bool			m_bPlayerHit;
		bool			mbAniHit;
		bool			m_DashAir;
		bool			m_AttackAri;
		bool			m_bHitAir;
		bool			m_bMagic;
		bool			m_bLeftWalkCheak;
		bool			m_bRightWalkCheak;

		static float	s_fCheakTime;
		static float	s_fTimer;
		static float	m_time;
		static float	s_fSkillTimer;
		static bool		m_bInvincibility;
		static Vector2	m_vFixPos;
		static bool		m_bSave;
		static bool		m_bStart;
		bool			m_bDashInvinci;
		bool			m_bPlayerIn;
		bool			m_bHp;
		bool			m_bHealSave;


		//Player 종류 별 이미지
		Texture*		m_pStateImg;
		Texture*		m_pPlayerDashEffect;
		Texture*		m_pPlayerAttackeffect;
		Texture*		m_pPlayerAttack_Up_Down_effect;
		Texture*		m_pPlayerHit;

		Sound*			m_pSAttack;
		Sound*			m_pSMagic;
		Sound*			m_pSDash;
		Sound*			m_pSWalk;
		Sound*			m_pSJump;
		Sound*			m_pSLand;
		Sound*			m_pSFloating;
		Sound*			m_pSHeal;
		Sound*			m_pSHeal1;
		Sound*			m_pSHit;
		Sound*			m_pDeath;

		State			m_tPlayer;
		Player_Action	m_tPlayerAction;
		Animator*		mAnimator;
		Animator*		mAttackAnimator;

		Vector2			mPlayerPos;
		Vector2			mJumpStopPos;
		Vector2			mUpDown_Pos;
		Vector2			mUpDownStop_Pos;
		

		int				m_iAniHit;
		int				mHp;
		int				mMp;
	};

}