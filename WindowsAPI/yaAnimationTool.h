#pragma once
#include "yaScene.h"


namespace ya
{
	class Texture;
	class AnimationTool
		: public Scene
	{
	public:
		AnimationTool();
		~AnimationTool();

	public:

		void Initialize() override;
		void Tick() override;
		void Render(HDC hdc) override;

		virtual void Enter() override;
		virtual void Exit() override;

	private:

	

	


	};



}
