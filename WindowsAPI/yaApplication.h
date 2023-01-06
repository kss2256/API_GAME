#pragma once
#include "Common.h"

namespace ya
{
	// 메인 프로그램이 될 녀석
	class Application
	{
	public:
		static Application& GetInstance()
		{
			static Application mInstance;
			return mInstance;
		}

		void Initialize(WindowData data);
		void initializeAtlasWindow(WindowData data);
		void Tick();

		void SetMenuBar(bool power);

		static void SetVideo(bool walk) { m_bVideoCheak = walk; }
		static bool GetVideo() { return m_bVideoCheak; }
		

		WindowData GetWindowData() { return mWindowData; }
		WindowData GetAtlasWindowData() { return mAtlasWindowData; }
		HDC GetHdc() { return mWindowData.hdc; }
		HPEN GetPen(ePenColor color) { return mPens[(UINT)color]; }
		HBRUSH GetBrush(eBrushColor color) { return mBrushes[(UINT)color]; }
		eSceneType GetPlaySceneType();


	private:
		Application();
		~Application();

		void initializeWindow(WindowData data);


	private:
		
		static bool m_bVideoCheak;

		WindowData mWindowData;
		WindowData mAtlasWindowData;

		HPEN mPens[(UINT)ePenColor::End];
		HBRUSH mBrushes[(UINT)eBrushColor::End];

		HMENU mMenu;
	};
}
