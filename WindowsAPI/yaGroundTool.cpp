#include "framework.h"
#include "WindowsAPI.h"
#include "yaGroundTool.h"
#include "yaApplication.h"
#include "yaSceneManager.h"
#include "yaCollisionManager.h"
#include "yaScene.h"
#include "yaImage.h"
#include "yaInput.h"
#include "yaCamera.h"
#include "yaResources.h"
#include "yaUIManager.h"
#include "yaPlayer.h"
#include "yaGround.h"
#include "yaObject.h"
#include "yaHusk.h"
#include "yaBaldur.h"
#include "yaHornet.h"
#include "yaPlayer_Save.h"

#include "yaCollider.h"
#include "yaComponent.h"
#include "yaGameObject.h"
#include "yaObject.h"
#include "yaSound.h"

//#include "framework.h"
//#include "WindowsAPI.h"

namespace ya
{



	GroundTool::GroundTool()
		: m_Img(nullptr)
		, m_Sound(nullptr)
		, m_pPlayer(nullptr)
	{
		m_Img = Resources::Load<Texture>(L"Map", L"..\\Resources\\Image\\Map.bmp");
		Player_Save* save = ya::object::Instantiate<Player_Save>(eColliderLayer::GameObject);
		
		


		//m_pPlayer = ya::object::Instantiate<Player>(eColliderLayer::Player);
		//m_pPlayer->SetPos({ 8613.0f, 3750.0f });
		//m_pPlayer->SetPos(Vector2(500.f, 900.f));
		//m_pPlayer->SetResurrection(true);
		//Camera::SetTarget(m_pPlayer);
	}

	GroundTool::~GroundTool()
	{

	}

	void GroundTool::Initialize()
	{


		

	}

	void GroundTool::Tick()
	{

		
		if (KEY_DOWN(eKeyCode::ESC))
		{
			
			
			//m_pPlayer = ya::object::Instantiate<Player>(eColliderLayer::Player);

			//Hornet* hornet = ya::object::Instantiate<Hornet>(eColliderLayer::Monster);
			//hornet->SetTarget(player);

			//Husk* husk = ya::object::Instantiate<Husk>(eColliderLayer::Monster);
			//husk->SetTarget(player);

			//Baldur* baldur = ya::object::Instantiate<Baldur>(eColliderLayer::Monster);
			//baldur->SetTarget(player);

			ya::SceneManager::ChangeScene(eSceneType::Logo);
			

		}

		

		Scene::Tick();
	}

	void GroundTool::Render(HDC hdc)
	{

		Vector2 Pos = Camera::CalculatePos(Vector2(0.f, 0.f));

		TransparentBlt(hdc
			, (int)Pos.x
			, (int)Pos.y
			, m_Img->GetWidth(), m_Img->GetHeight()
			, m_Img->GetDC(), 0, 0
			, m_Img->GetWidth(), m_Img->GetHeight(), RGB(255, 0, 255));
		TILE_SIZE;

		//WindowData mainWidnow = Application::GetInstance().GetWindowData();

		//HPEN redPen = CreatePen(PS_SOLID, 2, RGB(0, 125, 0));
		//HPEN oldPen = (HPEN)SelectObject(hdc, redPen);

		//int maxRow = mainWidnow.height / TILE_SIZE * TILE_SCALE + 1;
		//for (size_t i = 0; i < maxRow; i++)
		//{
		//	MoveToEx(hdc, 0, (int)(TILE_SIZE * i * TILE_SCALE), nullptr);
		//	LineTo(hdc, mainWidnow.width, (int)(TILE_SIZE * i * TILE_SCALE));
		//}

		//int maxColumn = mainWidnow.width / TILE_SIZE * TILE_SCALE + 1;
		//for (size_t i = 0; i < maxColumn; i++)
		//{
		//	MoveToEx(hdc, (int)(TILE_SIZE * i * TILE_SCALE), 0, nullptr);
		//	LineTo(hdc, (int)(TILE_SIZE * i * TILE_SCALE), mainWidnow.height);
		//}

		//(HPEN)SelectObject(hdc, oldPen);
		//DeleteObject(redPen);

		Scene::Render(hdc);

	}

	void GroundTool::Enter()
	{

		Scene::Initialize();
		Application::GetInstance().SetMenuBar(true);
		UIManager::Pop(eUIType::START);
		UIManager::Pop(eUIType::HP);
		UIManager::Pop(eUIType::ANIMATIONPANEL);
		UIManager::Push(eUIType::GROUNDBUTTON);

	
	
		CollisionManager::SetLayer(eColliderLayer::Ground, eColliderLayer::Player, true);
		CollisionManager::SetLayer(eColliderLayer::Ground, eColliderLayer::Monster, true);
		CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, true);
		CollisionManager::SetLayer(eColliderLayer::Monster_Projecttile, eColliderLayer::Player_Projecttile, true);
		CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, true);
		CollisionManager::SetLayer(eColliderLayer::Monster_Projecttile, eColliderLayer::Player, true);
		CollisionManager::SetLayer(eColliderLayer::Player, eColliderLayer::GameObject, true);


	}

	void GroundTool::Exit()
	{
		
	}

}

