#pragma once
#include "yaGameObject.h"


namespace ya
{
	class Player;
	class Texture;
	class BgImageObject : public GameObject
	{
	public:
		BgImageObject();
		~BgImageObject();

		virtual void Initialize() override;
		virtual void Tick() override;
		virtual void Render(HDC hdc) override;

		void SetImage(const std::wstring& key, const std::wstring& fileName);


		Player* mPlayer;


	private:
		Texture* mImage;
		Texture* mPixelImage;
	};
}
