#pragma once
#include "yaGameObject.h"

namespace ya
{
	class Sound;
	class Animator;
	class False_Key
		: public GameObject
	{

	private:

		static float	s_fTimer;
		bool			m_bCheak;
		bool			m_bEnd;

		Animator*		mAnimator;

		Sound*			m_Sound;

	public:

		virtual void Tick() override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;

	public:
		False_Key();
		~False_Key();


	};


}
