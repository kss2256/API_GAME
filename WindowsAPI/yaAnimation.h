#pragma once
#include "yaEntity.h"

namespace ya
{
	class Player;
	class Baldur;
	class Texture;
	class Animator;
	class Animation : public Entity
	{
	public:

		Animation();
		~Animation();

		void Tick();
		void Render(HDC hdc);
		void Create(Texture* image
			, Vector2 leftTop, Vector2 size, Vector2 offset
			, UINT spriteLegth, float duration, bool bAffectedCamera = true);
		void CreateAnima(const std::wstring& name, Texture* image, std::vector<Sprite> frm, bool bAffectedCamera = true);




		void Reset();
		bool isComplete() { return mbComplete; }
		void SetAnimator(Animator* animator) { mAnimator = animator; }


		void SetAnirender(bool Anirender) { mbAni = Anirender; }
		bool GetAnirender() { return mbAni; }



	private:
		Animator* mAnimator;
		Texture* mImage;
		std::vector<Sprite> mSpriteSheet;
		UINT mSpriteIndex;
		float mTime;
		float m_time;
		int transparency;

		Player* m_pPlayerAniHit;
		Baldur*	m_pBaldurAniHit;

		bool mStart;
		bool mbComplete;
		bool mEnd;
		bool mAffectedCamera;
		bool mbAni;



	};

}