#pragma once
#include "yaComponent.h"
#include "yaAnimation.h"

namespace ya
{
	class Texture;
	class Animator : public Component
	{
	public:
		struct Event
		{
			//함수 포인터
			void operator=(std::function<void()> func)
			{
				mEvent = std::move(func);
			}

			void operator()()
			{
				if (mEvent)
					mEvent();
			}


			std::function<void()> mEvent;
		};
		struct Events
		{
			Event mStartEvent;
			Event mCompleteEvent;
			Event mEndEvent;
		};

		Animator();
		~Animator();

		virtual void Tick() override;
		virtual void Render(HDC hdc) override;

		std::wstring CreateAniamtionKey(std::wstring path);
		Animation* FindAnimation(const std::wstring& name);
		void CreateAnimation(const std::wstring& name, Texture* image
			, Vector2 leftTop, Vector2 size, Vector2 offset
			, UINT spriteLegth, float duration, bool bAffectedCamera = true);
		void PushAnimation(const std::wstring& name, Texture* image, std::vector<Sprite> frm, bool bAffectedCamera = true);
		void SaveAnima(const std::wstring& name, const std::wstring& path, Texture* image, std::vector<Sprite> frm, bool bAffectedCamera = true);
		void LoadAnima(const std::wstring& name);

		void CreateAnimations(const std::wstring& path, const std::wstring& name, Vector2 offset = Vector2::Zero, float duration = 0.1f);

		void Play(const std::wstring& name, bool bLoop = false);

		void SetAnirender(bool Anirender) { mbAni = Anirender; }
		bool GetAnirender() { return mbAni; }



		Events* FindEvents(const std::wstring key);
		std::function<void()>& GetStartEvent(const std::wstring key);
		std::function<void()>& GetCompleteEvent(const std::wstring key);
		std::function<void()>& GetEndEvent(const std::wstring key);

	private:
		std::map<std::wstring, Animation*> mAnimations;
		std::map<std::wstring, Events*> mEvents;
		std::vector<Sprite>				mAnifrm;

		Animation* mPlayAnimaion;
		bool mbLoop;
		bool mbAni;


		Texture* mSPriteSheet;
	};
}
