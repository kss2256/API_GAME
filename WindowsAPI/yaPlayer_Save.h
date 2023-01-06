#pragma once
#include "yaGameObject.h"

namespace ya
{
	class Sound;
	class Texture;
	class Player_Save
		: public GameObject
	{
	private:

		Texture*	m_Img;
		Sound*		m_pSound;

	public:

		virtual void Tick() override;
		virtual void Render(HDC _hdc) override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;

	public:
		Player_Save();
		~Player_Save();


	};


}

