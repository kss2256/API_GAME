#pragma once
#include "yaGameObject.h"

namespace ya
{
	class Texture;
	class Tile : public GameObject
	{
	public:
		Tile(Vector2 pos);
		~Tile();

		void Initiailize(Texture* atlas, int index);
		virtual void Tick() override;
		virtual void Render(HDC hdc) override;

		void SetIndex(UINT index);
		UINT GetIndex() { return mIndex; }

	private:
		Texture* mAtlas;
		UINT mIndex;
		UINT mX;
		UINT mY;
	};

}