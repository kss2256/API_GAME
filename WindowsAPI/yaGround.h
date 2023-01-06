#pragma once
#include "yaGameObject.h"

namespace ya
{
	class Ground : public GameObject
	{
	public:
		Ground();
		~Ground();

		virtual void Tick() override;
		virtual void Render(HDC hdc) override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;

		void SetCheak(bool cheak) { m_Cheak = cheak; }
		bool GetCheak() { return m_Cheak; }

	private:

		Vector2 mPos;
		Vector2 mScale;

		bool m_Cheak;


	};
}
