#pragma once
#include "yaButton.h"

namespace ya
{
	class Texture;
	class GroundButton
		: public Button
	{


	private:
		eGroundType		m_eGround;

		Vector2			m_vStartPos;
		Vector2			m_vEndPos;


		bool			m_bLineCheak;
		bool			m_bLineRender;

	public:

		virtual void Click() override;
		virtual void MouseDown() override;
		virtual void MouseOn() override;
		virtual void OnRender(HDC hdc) override;

	public:

		void SetGround(eGroundType _TYPE) { m_eGround = _TYPE; }
		eGroundType GetGround() { return m_eGround; }

	public:
		GroundButton(eUIType type);
		~GroundButton();


	};


}
