#include "yaGroundButton.h"
#include "yaResources.h"
#include "yaSceneManager.h"
#include "yaScene.h"
#include "yaCamera.h"
#include "yaEarth.h"
#include "yaInput.h"
#include "yaImage.h"
#include "yaGroundTool.h"
#include "yaGroundEarth.h"
#include "yaLineGround.h"


#define X_SIZE 217.5
#define Y_SIZE 48

namespace ya
{

	GroundButton::GroundButton(eUIType type)
		: Button(type)
		, m_eGround(eGroundType::END)
		, m_vStartPos(Vector2::Zero)
		, m_vEndPos(Vector2::Zero)
		, m_bLineCheak(false)
		, m_bLineRender(false)
	{


	}

	GroundButton::~GroundButton()
	{
	}

	void GroundButton::Click()
	{


		Vector2 Pos = GetPos();
		Pos.x += 1380.f;
		Pos.y += 10.f;		
		Vector2 MousePos = MOUSE_POS;
		m_vEndPos = MOUSE_POS;

		m_vEndPos /= TILE_SIZE;

		if (MousePos.x >= Pos.x && MousePos.y >= Pos.y
			&& MousePos.x <= Pos.x + X_SIZE && MousePos.y <= Pos.y + Y_SIZE)
		{
			m_eGround = eGroundType::Ground;
		}

		if (MousePos.x >= Pos.x + X_SIZE && MousePos.y >= Pos.y
			&& MousePos.x <= Pos.x + X_SIZE * 2 && MousePos.y <= Pos.y + Y_SIZE)
		{
			m_eGround = eGroundType::Ground1;
		}
		if (MousePos.x >= Pos.x && MousePos.y >= Pos.y + Y_SIZE
			&& MousePos.x <= Pos.x + X_SIZE && MousePos.y <= Pos.y + Y_SIZE * 2)
		{
			m_eGround = eGroundType::Ground2;
		}
		if (MousePos.x >= Pos.x + X_SIZE && MousePos.y >= Pos.y + Y_SIZE
			&& MousePos.x <= Pos.x + X_SIZE * 2 && MousePos.y <= Pos.y + Y_SIZE * 2)
		{
			m_eGround = eGroundType::Object;
		}
		if (MousePos.x >= Pos.x && MousePos.y >= Pos.y + Y_SIZE * 2
			&& MousePos.x <= Pos.x + X_SIZE && MousePos.y <= Pos.y + Y_SIZE * 3)
		{
			m_eGround = eGroundType::Object1;
		}
		if (MousePos.x >= Pos.x + X_SIZE && MousePos.y >= Pos.y + Y_SIZE * 2
			&& MousePos.x <= Pos.x + X_SIZE * 2 && MousePos.y <= Pos.y + Y_SIZE * 3)
		{
			m_eGround = eGroundType::Object2;
		}
		if (MousePos.x >= Pos.x && MousePos.y >= Pos.y + Y_SIZE * 3
			&& MousePos.x <= Pos.x + X_SIZE && MousePos.y <= Pos.y + Y_SIZE * 4)
		{
			m_eGround = eGroundType::Col;	
		}
		if (MousePos.x >= Pos.x + X_SIZE && MousePos.y >= Pos.y + Y_SIZE * 3
			&& MousePos.x <= Pos.x + X_SIZE * 2 && MousePos.y <= Pos.y + Y_SIZE * 4)
		{
			m_eGround = eGroundType::Rol;		
		}
		if (MousePos.x >= Pos.x && MousePos.y >= Pos.y + Y_SIZE * 4
			&& MousePos.x <= Pos.x + X_SIZE && MousePos.y <= Pos.y + Y_SIZE * 5)
		{
			m_eGround = eGroundType::Red_Col;
		}
		if (MousePos.x >= Pos.x + X_SIZE && MousePos.y >= Pos.y + Y_SIZE * 4
			&& MousePos.x <= Pos.x + X_SIZE * 2 && MousePos.y <= Pos.y + Y_SIZE * 5)
		{
			m_eGround = eGroundType::Red_Rol;
		}
		if (MousePos.x >= Pos.x && MousePos.y >= Pos.y + Y_SIZE * 5
			&& MousePos.x <= Pos.x + X_SIZE && MousePos.y <= Pos.y + Y_SIZE * 6)
		{
			m_eGround = eGroundType::Baldur;
		}
		if (MousePos.x >= Pos.x + X_SIZE && MousePos.y >= Pos.y + Y_SIZE * 5
			&& MousePos.x <= Pos.x + X_SIZE * 2 && MousePos.y <= Pos.y + Y_SIZE * 6)
		{
			m_eGround = eGroundType::Husk;
		}
		if (MousePos.x >= Pos.x && MousePos.y >= Pos.y + Y_SIZE * 6
			&& MousePos.x <= Pos.x + X_SIZE && MousePos.y <= Pos.y + Y_SIZE * 7)
		{
			m_eGround = eGroundType::Hornet;
		}
		if (MousePos.x >= Pos.x + X_SIZE && MousePos.y >= Pos.y + Y_SIZE * 6
			&& MousePos.x <= Pos.x + X_SIZE * 2 && MousePos.y <= Pos.y + Y_SIZE * 7)
		{
			m_eGround = eGroundType::False_Knight;
		}
		if (MousePos.x >= Pos.x && MousePos.y >= Pos.y + Y_SIZE * 7
			&& MousePos.x <= Pos.x + X_SIZE && MousePos.y <= Pos.y + Y_SIZE * 8)
		{
			m_eGround = eGroundType::Player;
		}
		if (MousePos.x >= Pos.x + X_SIZE && MousePos.y >= Pos.y + Y_SIZE * 7
			&& MousePos.x <= Pos.x + X_SIZE * 2 && MousePos.y <= Pos.y + Y_SIZE * 8)
		{
			//다음 캐릭터 자리
			m_eGround = eGroundType::Player;
		}

	
		GroundEarth* newGround = new GroundEarth();

		newGround->SetGround(m_eGround);
		newGround->SetPos(MousePos);
		
		
		Scene* playScene = SceneManager::GetPlayScene();
		playScene->AddGameObject(newGround, eColliderLayer::GroundPriview);
	




	}

	void GroundButton::MouseDown()
	{
		m_vStartPos = MOUSE_POS;
		
		m_vStartPos /= TILE_SIZE;

	}

	void GroundButton::MouseOn()
	{
	}

	void GroundButton::OnRender(HDC hdc)
	{

		BLENDFUNCTION func = {};
		func.BlendOp = AC_SRC_OVER;
		func.BlendFlags = 0;
		func.AlphaFormat = AC_SRC_ALPHA;
		func.SourceConstantAlpha = 255;

		if (nullptr != mImage)
		{
			AlphaBlend(hdc, (int)mScreenPos.x, (int)mScreenPos.y
				, mImage->GetWidth(), mImage->GetHeight()
				, mImage->GetDC(), 0, 0, mImage->GetWidth(), mImage->GetHeight(), func);
		}


	}

}