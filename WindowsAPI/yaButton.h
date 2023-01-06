#pragma once
#include "yaUiBase.h"


typedef void(ya::Entity::* DELEGATE)(void);


namespace ya
{
	class Sound;
	class LogoScene;
	class Texture;
	class Button : public UiBase
	{
	public:
		Button(eUIType type);
		virtual ~Button();

		virtual void OnInit() override;
		virtual void OnActive() override;
		virtual void OnInActive() override;
		virtual void OnTick() override;
		virtual void OnRender(HDC hdc) override;
		virtual void OnClear() override;


		virtual void Click();
		virtual void MouseDown();
		virtual void MouseOn();

		virtual void SetDelegate(Entity* _pInst, DELEGATE _Func)
		{
			m_pInst = _pInst;
			m_pDelegateFunc = _Func;
		}

		static bool SetChangeSound(bool walk) { m_bSound = walk; }


	private:
		Event			mOnClick;
		Event			mOnMouseDown;
		Texture*			m_pImg;
		eModeType		m_eMode;
		Vector2			m_VecPos;

		LogoScene*		m_pLogo;

		int				m_iOverlap;
		float			m_time;

		bool			mbMouseOn;
		bool			m_bOverlap;

		static bool		m_bSound;

		Entity*			m_pInst;
		DELEGATE        m_pDelegateFunc;

		Sound*			m_Sound;
	};

}