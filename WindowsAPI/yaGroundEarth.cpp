#include "yaGroundEarth.h"
#include "yaResources.h"
#include "yaSceneManager.h"
#include "yaScene.h"
#include "yaEarth.h"
#include "yaCamera.h"
#include "yaInput.h"
#include "yaImage.h"


namespace ya
{

	GroundEarth::GroundEarth()
		: m_eGround(eGroundType::END)
		, m_Img(nullptr)
		, m_pPlayer(nullptr)
		, m_pGround(nullptr)
		, m_pGround1(nullptr)
		, m_pGround2(nullptr)
		, m_pObject(nullptr)
		, m_pObject1(nullptr)
		, m_pObject2(nullptr)
		, m_pLineCol(nullptr)
		, m_pLineRol(nullptr)
		, m_pRedLineCol(nullptr)
		, m_pRedLineRol(nullptr)
		, m_pBaldur(nullptr)
		, m_pHusk(nullptr)
		, m_pHornet(nullptr)
		, m_pFalseKnight(nullptr)
		, m_vMousePos(Vector2::Zero)
		, m_vLineRender(Vector2::Zero)
		, m_vLineMouse(Vector2::Zero)
		, m_vStartPos(Vector2::Zero)
		, m_vEndPos(Vector2::Zero)
		, m_bRenderCheak(false)
		
	{
		m_pGround = Resources::Load<Texture>(L"Ground", L"..\\Resources\\Image\\Ground.bmp");
		m_pGround1 = Resources::Load<Texture>(L"Ground1", L"..\\Resources\\Image\\Ground1.bmp");
		m_pGround2 = Resources::Load<Texture>(L"Ground2", L"..\\Resources\\Image\\Ground2.bmp");
		m_pObject = Resources::Load<Texture>(L"Object", L"..\\Resources\\Image\\Object.bmp");
		m_pObject1 = Resources::Load<Texture>(L"Object1", L"..\\Resources\\Image\\Object1.bmp");
		m_pObject2 = Resources::Load<Texture>(L"Object2", L"..\\Resources\\Image\\Object2.bmp");
		m_pLineCol = Resources::Load<Texture>(L"Col", L"..\\Resources\\Image\\Col.bmp");
		m_pLineRol = Resources::Load<Texture>(L"Rol", L"..\\Resources\\Image\\Rol.bmp");
		m_pRedLineCol = Resources::Load<Texture>(L"Red_Col", L"..\\Resources\\Image\\Red_Col.bmp");
		m_pRedLineRol = Resources::Load<Texture>(L"Red_Rol", L"..\\Resources\\Image\\Red_Rol.bmp");
		m_pBaldur = Resources::Load<Texture>(L"Baldur_Prev", L"..\\Resources\\Image\\Baldur_Prev.bmp");
		m_pHusk = Resources::Load<Texture>(L"Husk_Prev", L"..\\Resources\\Image\\Husk_Prev.bmp");
		m_pHornet = Resources::Load<Texture>(L"Hornet_Prev", L"..\\Resources\\Image\\Hornet_Prev.bmp");
		m_pFalseKnight = Resources::Load<Texture>(L"False_Knight_Prev", L"..\\Resources\\Image\\False_Knight_Prev.bmp");
		m_pPlayer = Resources::Load<Texture>(L"Owner", L"..\\Resources\\Image\\Owner.bmp");
	}

	GroundEarth::~GroundEarth()
	{

	}



	void GroundEarth::Tick()
	{


		GameObject::Tick();
	}

	void GroundEarth::Render(HDC hdc)
	{
		Vector2 Pos = MOUSE_POS;

		switch (m_eGround)
		{
		case eGroundType::Ground:
			m_Img = m_pGround;
			break;
		case eGroundType::Ground1:
			m_Img = m_pGround1;
			break;
		case eGroundType::Ground2:
			m_Img = m_pGround2;
			break;
		case eGroundType::Object:
			m_Img = m_pObject;
			break;
		case eGroundType::Object1:
			m_Img = m_pObject1;
			break;
		case eGroundType::Object2:
			m_Img = m_pObject2;
			break;
		case eGroundType::Col:
			m_Img = m_pLineCol;
			break;
		case eGroundType::Rol:
			m_Img = m_pLineRol;
			break;
		case eGroundType::Red_Col:
			m_Img = m_pRedLineCol;
			break;
		case eGroundType::Red_Rol:
			m_Img = m_pRedLineRol;
			break;
		case eGroundType::Baldur:
			m_Img = m_pBaldur;
			break;
		case eGroundType::Husk:
			m_Img = m_pHusk;
			break;
		case eGroundType::Hornet:
			m_Img = m_pHornet;
			break;
		case eGroundType::False_Knight:
			m_Img = m_pFalseKnight;
			break;
		case eGroundType::Player:
			m_Img = m_pPlayer;
			break;

		case eGroundType::END:
			break;

		}
		


		if (m_eGround == eGroundType::Col || m_eGround == eGroundType::Rol
			|| m_eGround == eGroundType::Red_Col || m_eGround == eGroundType::Red_Rol)
		{

			
				if (KEY_DOWN(eKeyCode::LBTN))
				{
					m_vStartPos = Camera::CalRealPos(MOUSE_POS);

					m_vStartPos /= (TILE_SIZE * 2);
					m_vStartPos *= (TILE_SIZE * 2);
					m_vEndPos = m_vStartPos;
					m_vLineRender = m_vStartPos;
					m_vLineRender = Camera::CalculatePos(m_vLineRender);
					m_vLineMouse = m_vStartPos;
					m_bRenderCheak = true;
				}
				if (KEY_DOWN(eKeyCode::RBTN))
				{
					m_vEndPos = Camera::CalRealPos(MOUSE_POS);

					m_vEndPos /= (TILE_SIZE * 2);
					m_vEndPos *= (TILE_SIZE * 2);

					m_bRenderCheak = false;
				}
			

			HPEN GreenPen = (HPEN)CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
			HPEN OriginalPen = (HPEN)SelectObject(hdc, GreenPen);


			HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			HBRUSH hOriginBrush = (HBRUSH)SelectObject(hdc, hNullBrush);					

			if(m_bRenderCheak)
			{
				m_vLineMouse = MOUSE_POS;

				if (m_eGround == eGroundType::Col || m_eGround == eGroundType::Red_Col)
				{
					Rectangle(hdc
						, (int)m_vLineRender.x
						, (int)m_vLineRender.y
						, (int)m_vLineMouse.x
						, (int)m_vLineRender.y + 1);
				}
				if (m_eGround == eGroundType::Rol || m_eGround == eGroundType::Red_Rol)
				{
					Rectangle(hdc
						, (int)m_vLineRender.x
						, (int)m_vLineRender.y
						, (int)m_vLineRender.x + 1
						, (int)m_vLineMouse.y);
				}

			}

			GreenPen = (HPEN)SelectObject(hdc, OriginalPen);
			DeleteObject(GreenPen);
			SelectObject(hdc, hOriginBrush);

		}

		


		if (KEY_UP(eKeyCode::RBTN))
		{
			Earth* pground = new Earth;
			pground->SetGround(m_eGround);
			m_vMousePos = MOUSE_POS;
			pground->SetPos(Camera::CalRealPos(m_vMousePos));
			Scene* playScene = SceneManager::GetPlayScene();

			if (m_eGround == eGroundType::Col || m_eGround == eGroundType::Red_Col)
			{
				m_vMousePos.x = abs(m_vStartPos.x + m_vEndPos.x) / 2;
				m_vMousePos.y = m_vStartPos.y;
				pground->SetPos(m_vMousePos);
			}
			if (m_eGround == eGroundType::Rol || m_eGround == eGroundType::Red_Rol)
			{
				m_vMousePos.x = m_vStartPos.x;
				m_vMousePos.y = abs(m_vStartPos.y + m_vEndPos.y) / 2;
				pground->SetPos(m_vMousePos);

			}		

			pground->SetLineEnd(m_vEndPos);
			pground->SetLineStart(m_vStartPos);

			if (m_eGround == eGroundType::Baldur || m_eGround == eGroundType::Hornet || m_eGround == eGroundType::Player
				|| m_eGround == eGroundType::Husk || m_eGround == eGroundType::False_Knight)
			{
				if(m_eGround == eGroundType::Player)
				{
					playScene->AddGameObject(pground, eColliderLayer::Player);
					Death();
				}
				else
				{
					playScene->AddGameObject(pground, eColliderLayer::Default);
					Death();
				}
			}
			else
			{
				playScene->AddGameObject(pground, eColliderLayer::Ground);
				Death();
			}

		}

		if(m_Img != nullptr)
		{
			TransparentBlt(hdc
				, (int)Pos.x - m_Img->GetWidth() / 2
				, (int)Pos.y - m_Img->GetHeight() / 2
				, m_Img->GetWidth(), m_Img->GetHeight()
				, m_Img->GetDC(), 0, 0, m_Img->GetWidth(), m_Img->GetHeight(), RGB(255, 0, 255));
		}


		GameObject::Render(hdc);

	}



}