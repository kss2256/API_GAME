#pragma once
#include "Common.h"
#include "yaUiBase.h"


namespace ya
{
	/// <summary>
	/// 플레이어의 조작에 영향을 받지 않죠 (위치 크기등등)
	/// 전체 화면 UI
	/// 기본 UI 
	/// HUD 이미지와 정보만 잇는 UI
	/// 버튼
	/// 내 UI 내 자식구조로 여러가지 다른 ui들을 구성해서
	/// 같이 움직이는 하나의 큰 UI 도 될수 있따.
	/// </summary>
	class UIManager
	{
	public:
		static void Initialize();
		static void OnLoad(eUIType type);
		static void Tick();
		static void Render(HDC hdc);
		static void OnComplete(UiBase* addUI);
		static void OnFail();
		static void Release();
		static void AnimationImage();

		static void Push(eUIType type);
		static void Pop(eUIType type);

		template <typename T>
		static T* GetUiInstant(eUIType type)
		{
			return dynamic_cast<T*>(mUIs[type]);
		}


	private:
		static void LogoInitialize();
		static void GroundInitialize();
		static void AnimationInitialize();
		static void PlayInitialize();

	private:
		static std::unordered_map<eUIType, UiBase*> mUIs;
		static std::queue<eUIType> mRequestUIQueue;
		static std::stack<UiBase*> mUIBases;
		static UiBase* mCurrentData;
		static AnimationBtuuon* m_Cheak;
	};
}
