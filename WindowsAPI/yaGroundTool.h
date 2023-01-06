#pragma once
#include "yaScene.h"

namespace ya
{
	class Sound;
	class Player;
	class Texture;
	class GroundTool : public Scene
	{
	public:
		GroundTool();
		~GroundTool();

	public:
		void Initialize() override;
		void Tick() override;
		void Render(HDC hdc) override;

		virtual void Enter();
		virtual void Exit();



		//void SetPos(Vector2 pos) { m_Pos = pos; }
		//Vector2 GetPos() { return m_Pos; }
		//void SetScale(Vector2 scale) { m_Scale = scale; }
		//Vector2 GetScale() { return m_Scale; }

	private:

		//Vector2 m_Pos;
		//Vector2 m_Scale;

		Texture* m_Img;

		Player* m_pPlayer;

		Sound*	m_Sound;


	};

}