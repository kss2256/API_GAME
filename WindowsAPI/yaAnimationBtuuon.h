#pragma once
#include "yaButton.h"



namespace ya
{
	class Texture;
	class Animator;

	class AnimationBtuuon
		: public Button
	{

	private:

		Texture*						m_pImg;
		Texture*						m_pSaveImg;
		Texture*						m_pLoadImg;
		Animator*					m_pmator;
		


		Vector2						m_vDownPos;
		Vector2						m_vClickedPos;
		Vector2						m_vDownrenderPos;
		Vector2						m_vClickedrenderPos;
		Vector2						m_vButtonPos;


		Vector2						m_FinalPos;
		Vector2						m_FinalScale;

		std::vector<Sprite>			m_vecfrm;
		std::wstring				m_AniPath;
		
		static float				m_fDuration;

	public:

		virtual void OnTick() override;

		virtual void Click() override;
		virtual void MouseDown() override;
		virtual void MouseOn() override;
		virtual void OnRender(HDC hdc) override;

	public:

		void SaveButton();
		void LoadButton();


	public:
		AnimationBtuuon(eUIType type);
		~AnimationBtuuon();



	};

}


