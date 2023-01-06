#pragma once
#include "Common.h"


namespace ya
{
	class Texture;
	class GameObject;
	class Camera
	{
	public:
		static void Initialize();
		static void Tick();
		static void Render(HDC hdc);
		static Vector2 CalculatePos(Vector2 pos) { return pos - mDistance; }
		static Vector2 CalRealPos(Vector2 pos) { return pos + mDistance; }
		static Vector2 CameraPos() { return mDistance; }
		static void SetTarget(GameObject* gameObj) { mTarget = gameObj; }
		static void SetCameraEffect(eCameraEffect effect) { mEffect = effect; }

		static Vector2 GetCameraLook() { return mLookPosition; }
		static void SetCameraLook(Vector2 cam) { mLookPosition = cam; }

		static bool GetShock() { return m_bShockCheak; }
		static void SetShock(bool cheak) { m_bShockCheak = cheak; }

		static bool GetFalse() { return m_False; }
		static void SetFalse(bool cheak) { m_False = cheak; }

		static bool GetHornet() { return m_Hornet; }
		static void SetHornet(bool cheak) { m_Hornet = cheak; }

	private:
		Camera() = default;
		~Camera() = default;

	private:
		static Vector2 mResolution; // 화면 해상도
		static Vector2 mLookPosition;
		static Vector2 mDistance; // 해상도 중심좌표와 현제 카메라의 간격차
		static Vector2 m_vFixPos;
		static Vector2 m_vShockPos;
		static Vector2 m_vMovePos;
	


		static GameObject* mTarget;

		static eCameraEffect mEffect;
		static Texture* mCutton;    //검정색 이미지
		static float mAlphaTime;
		static float mCuttonAlpha;
		static float mEndTime;

		static float m_fShockTime;
		static bool	 m_bShockCheak;


		static bool m_Cheak;
		static bool m_False;
		static bool m_Hornet;
	};


}