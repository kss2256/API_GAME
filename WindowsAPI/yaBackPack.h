#pragma once
#include "yaGameObject.h"

namespace ya
{
	class Texture;
	class BackPack : public GameObject
	{
	public:
		BackPack();
		~BackPack();

		virtual void Tick() override;
		virtual void Render(HDC hdc) override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;

	private:
		float mSpeed;
		Texture* mImage;
		//int alpha;
	};
}

