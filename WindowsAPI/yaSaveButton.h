#pragma once
#include "yaButton.h"




namespace ya
{

	class SaveButton
		: public Button
	{
	private:



	public:

		virtual void Click() override;
		virtual void MouseDown() override;
		virtual void MouseOn() override;
		virtual void OnRender(HDC hdc) override;


	public:
		SaveButton(eUIType type);
		~SaveButton();


	};


}

