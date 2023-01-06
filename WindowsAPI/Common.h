#pragma once

#include "framework.h"

#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <list>
#include <map>
#include <set>
#include <bitset>
#include <functional>
#include <filesystem>
#include <assert.h>

#include "def.h"
#include "Maths.h"
#include "func.h"

// 렌더링 라이브러리
#pragma comment(lib, "Msimg32.lib")

#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

//typedef Pos POINT;
//using Pos = POINT;

//typedef std::vector<std::vector<GameObject*>> GameObjects;
//typedef std::vector<GameObject*> LayerObjects;

union Pixel
{
	struct
	{
		BYTE R;
		BYTE G;
		BYTE B;
		BYTE A;
	};
	DWORD Color;

	Pixel(BYTE r, BYTE g, BYTE b, BYTE a)
		:R(r), G(g), B(b), A(a)
	{

	}

	Pixel(COLORREF color)
		:Color(color)
	{

	}

};

struct WindowData
{
	// 렌더 타겟
	// 프레임버퍼

	HWND hWnd;
	HDC hdc;
	HBITMAP backTexture;
	HDC backBuffer;
	UINT height;
	UINT width;

	void clear()
	{
		hWnd = nullptr;
		hdc = nullptr;
		height = 0;
		width = 0;
	}
};

struct Pen
{
private:
	HDC mHdc;
	HPEN mOldPen;
	HPEN mPen;

public:
	Pen(HDC hdc)
		: mHdc(hdc)
		, mOldPen(NULL)
		, mPen(NULL)
	{

	}

	Pen(HDC hdc, HPEN pen)
		: mHdc(hdc)
		, mOldPen(NULL)
		, mPen(pen)
	{
		mOldPen = (HPEN)SelectObject(mHdc, pen);
	}

	~Pen()
	{
		SelectObject(mHdc, mOldPen);
		DeleteObject(mPen);
	}

	void SetPen(HPEN pen)
	{
		mOldPen = (HPEN)SelectObject(mHdc, pen);
	}

};

struct Brush
{
private:
	HDC mHdc;
	HBRUSH mOldBrush;
	HBRUSH mBrush;

public:
	Brush(HDC hdc, HBRUSH brush)
		: mHdc(hdc)
		, mOldBrush(NULL)
		, mBrush(brush)
	{
		mOldBrush = (HBRUSH)SelectObject(mHdc, brush);
	}

	~Brush()
	{
		SelectObject(mHdc, mOldBrush);
		DeleteObject(mBrush);
	}
};