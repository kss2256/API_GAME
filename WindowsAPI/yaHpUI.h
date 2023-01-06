#pragma once
#include "yaUiBase.h"

namespace ya
{
	class GameObject;
	class HpUI
		: public UiBase
	{
	private:

		GameObject* mTarget;



	public:
		virtual void OnInit() override;
		virtual void OnActive() override;
		virtual void OnInActive() override;
		virtual void OnTick() override;
		virtual void OnRender(HDC hdc) override;
		virtual void OnClear() override;

		void SetTarget(GameObject* target) { mTarget = target; }	



	public:
		HpUI(eUIType type);
		~HpUI();


	};


}
