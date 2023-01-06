#include "yaCamera.h"
#include "yaApplication.h"
#include "yaGameObject.h"
#include "yaInput.h"
#include "yaTime.h"
#include "yaImage.h"

namespace ya
{
	Vector2 Camera::mResolution = Vector2::Zero;
	Vector2 Camera::mLookPosition = Vector2::Zero;
	Vector2 Camera::mDistance = Vector2::Zero;
	Vector2 Camera::m_vFixPos = Vector2::Zero;
	Vector2 Camera::m_vShockPos = Vector2::Zero;
	Vector2 Camera::m_vMovePos = Vector2::Zero;
	GameObject* Camera::mTarget = nullptr;

	eCameraEffect Camera::mEffect = eCameraEffect::None;;
	Texture* Camera::mCutton = nullptr;
	float Camera::mAlphaTime = 0.0f;
	float Camera::mCuttonAlpha = 0.0f;
	float Camera::mEndTime = 4.0f;
	float Camera::m_fShockTime = 0.0f;
	bool Camera::m_bShockCheak = false;
	bool Camera::m_Cheak = false;
	bool Camera::m_False = false;
	bool Camera::m_Hornet = false;

	void Camera::Initialize()
	{
		WindowData data = Application::GetInstance().GetWindowData();
		mResolution = Vector2((float)data.width, (float)data.height);
		mLookPosition = (mResolution / 2.0f);

		//mEffect = eCameraEffect::FadeOut;
		mCutton = Texture::Create(L"CameraCutton", 1920, 1080);
		

	}

	void Camera::Tick()
	{
		if (mAlphaTime <= mEndTime)
		{
			mAlphaTime += Time::DeltaTime();
			float ratio = (mAlphaTime / mEndTime);

			if (eCameraEffect::FadeIn == mEffect)
			{
				mCuttonAlpha = 1.0f - ratio;
			}
			else if (eCameraEffect::FadeOut == mEffect)
			{
				mCuttonAlpha = ratio;
			}
		}
		if (mAlphaTime > mEndTime)
		{
			mEffect = eCameraEffect::None;
			mAlphaTime = 0.f;
		}

		if (m_bShockCheak)
		{
			
			m_fShockTime += DT;
			
			if (m_fShockTime < 0.1)
			{
				if (mTarget != nullptr)
				m_vShockPos = mTarget->GetPos();
				m_vMovePos = m_vShockPos;
				
			}
			if (m_fShockTime >= 0.1 && m_fShockTime <= 0.7)
			{
				if (m_vShockPos.x + 10 > m_vMovePos.x)
				{
					
					mLookPosition.x += 600.0f * Time::DeltaTime();
					m_vMovePos.x += 600.0f * Time::DeltaTime();
				}
				else
				{
					
					mLookPosition.x -= 600.0f * Time::DeltaTime();
					m_vMovePos.x -= 600.0f * Time::DeltaTime();
				}
			}

			if(m_fShockTime > 0.7)
			{
				m_bShockCheak = false;
				m_vShockPos = {};
				m_fShockTime = 0.f;
			}
		}


		if (KEY_PRESSE(eKeyCode::W))
		{
			mLookPosition.y -= 600.0f * Time::DeltaTime();
		}
		if (KEY_PRESSE(eKeyCode::S))
		{
			mLookPosition.y += 600.0f * Time::DeltaTime();
		}
		if (KEY_PRESSE(eKeyCode::A))
		{
			mLookPosition.x -= 600.0f * Time::DeltaTime();
		}
		if (KEY_PRESSE(eKeyCode::D))
		{
			mLookPosition.x += 600.0f * Time::DeltaTime();
		}

		if (m_False)
		{

			if (mTarget->GetPos().y >= 3952 && mTarget->GetPos().x <= 960)
			{
				mLookPosition.x = 960;
				mLookPosition.y = 3952;
			}
			else if (mTarget->GetPos().y >= 3952)
			{
				if (mTarget->GetPos().x >= 1318)
				{
					mLookPosition.y = 3952;
					mLookPosition.x = 1318;
				}
				else 
				{
					mLookPosition.x = mTarget->GetPos().x;
					mLookPosition.y = 3952;
				}
			}

			mDistance = mLookPosition - (mResolution / 2.0f);
			return;
		}

		if (m_Hornet)
		{

			mLookPosition = Vector2(3500.f, 3952.f);



			mDistance = mLookPosition - (mResolution / 2.0f);
			return;
		}



		if (mTarget != nullptr)
		{
			if (m_Cheak == false)
			{
				mLookPosition = mTarget->GetPos();
				m_vFixPos = mTarget->GetPos();
				m_Cheak = true;
			}
			
			if (mTarget->GetPos().y >= 3952 || mTarget->GetPos().y <= 540
				|| mTarget->GetPos().x >= 10686 || mTarget->GetPos().x <= 960)
			{

				//if (mTarget->GetPos().y >= 3952 && mTarget->GetPos().x <= 960)
				//{
				//	mLookPosition.x = 960;
				//	mLookPosition.y = 3952;
				//}
				//else if (mTarget->GetPos().y >= 3952)
				//{
				//	mLookPosition.x = mTarget->GetPos().x;
				//	mLookPosition.y = 3952;
				//}
				//else if (mTarget->GetPos().x <= 960)
				//{
				//	mLookPosition.y = mTarget->GetPos().y;
				//	mLookPosition.x = 960;
				//}
				//
				//if (mTarget->GetPos().y <= 540)
				//{
				//	mLookPosition.x = mTarget->GetPos().x;
				//	mLookPosition.y = 540;
				//}
				//else if (mTarget->GetPos().x >= 10686)
				//{
				//	mLookPosition.y = mTarget->GetPos().y;
				//	mLookPosition.x = 10868;
				//}


				
				//mDistance = mLookPosition - (mResolution / 2.0f);
				return;
			}


			if (m_vFixPos.x != mTarget->GetPos().x || m_vFixPos.y != mTarget->GetPos().y)
				mLookPosition = mTarget->GetPos();

			m_vFixPos = mTarget->GetPos();
		}
		
		
		mDistance = mLookPosition - (mResolution / 2.0f);
	}

	void Camera::Render(HDC hdc)
	{
		if (mCuttonAlpha <= 0.0f)
			return;

		BLENDFUNCTION func = {};
		func.BlendOp = AC_SRC_OVER;
		func.BlendFlags = 0;
		func.AlphaFormat = 0;
		func.SourceConstantAlpha = (BYTE)(255.0f * mCuttonAlpha);

		AlphaBlend(hdc, 0, 0
			, mCutton->GetWidth(), mCutton->GetHeight()
			, mCutton->GetDC(), 0, 0
			, mCutton->GetWidth(), mCutton->GetHeight()
			, func);
	}
}