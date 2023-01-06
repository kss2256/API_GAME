#pragma once
#include "yaGameObject.h"

namespace ya
{
	class Animator;
	class AniPreview
		: public GameObject
	{

	private:

		std::wstring		m_wstrName;
		std::wstring		m_wstrPath;
		Animator*			mAnimator;

		bool				m_brenderoff;

	public:

	
		virtual void Tick() override;
		virtual void Render(HDC hdc) override;


		void SetAniname(std::wstring name) { m_wstrName = name; }
		void SetAnipath(std::wstring path) { m_wstrPath = path; }
		std::wstring GetAniname() { return m_wstrName; }
		std::wstring GetAnipath() { return m_wstrPath; }
		

	public:
		AniPreview();
		~AniPreview();

	};

}

