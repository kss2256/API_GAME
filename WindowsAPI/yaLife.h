#pragma once
#include "yaUiBase.h"

namespace ya
{
	class GameObject;
	class Life
		: public UiBase
	{
	private:
		GameObject* mTarget;


	public:

		virtual void OnRender(HDC hdc) override;

		void SetTarget(GameObject* target) { mTarget = target; }


	public:
		Life(eUIType type);
		~Life();


	};


}
