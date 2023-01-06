#pragma once
#include "yaScene.h"

namespace ya
{
	class Sound;
	class Texture;
	class Player;
	class PlayScene : public Scene
	{
	public:
		PlayScene();
		~PlayScene();

		void Initialize() override;
		void Tick() override;
		void Render(HDC hdc) override;


		virtual void Enter();
		virtual void Exit();

	private:


		State	m_sAi;
		Player* m_pPlayer;
		Texture* m_Img;

		Sound*	m_pHornetBgm;
		Sound*	m_pFalseBgm;
		Sound*	m_pBackBgm;



	};

}
