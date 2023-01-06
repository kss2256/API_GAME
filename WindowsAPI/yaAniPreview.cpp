#include "yaAniPreview.h"
#include "yaAnimator.h"
#include "yaInput.h"

namespace ya
{
	AniPreview::AniPreview()
		: m_brenderoff(false)
	{
	
	}

	AniPreview::~AniPreview()
	{

	}



	void AniPreview::Tick()
	{
		if (KEY_UP(eKeyCode::ENTER))
		{
			if(nullptr == mAnimator)
			mAnimator = new Animator();

			mAnimator->LoadAnima(GetAnipath());
			mAnimator->SetAnirender(true);
			mAnimator->Play(GetAniname(), true);

			AddComponent(mAnimator);			
		}
		
		if (KEY_UP(eKeyCode::SPACE))
		{
			Death();
			
		}

		
		GameObject::Tick();



	}

	void AniPreview::Render(HDC hdc)
	{

		GameObject::Render(hdc);


	}



}