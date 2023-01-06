#pragma once
#include "yaScene.h"

namespace ya
{

	class Texture;
	class LogoScene : public Scene
	{
	public:
		LogoScene();
		~LogoScene();

		void Initialize() override;
		void Tick() override;
		void Render(HDC hdc) override;

		virtual void Enter();
		virtual void Exit();

	public:

		void Start();
		void Ground();
		void Animation();

	private:

		float               m_fChange;
		bool                m_bChange;
		eModeType			m_eMode;
		Texture*			mImage;
		Texture*			mCheak;
		Sound*				mSound;
		Sound*				mSound1;
	};

}


