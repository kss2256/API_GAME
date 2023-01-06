#include "yaPlayScene.h"
#include "yaPlayer.h"
#include "yaBaldur.h"
#include "yaHusk.h"
#include "yaInput.h"
#include "yaEarth.h"
#include "yaImage.h"
#include "yaResources.h"
#include "yaCamera.h"
#include "yaSceneManager.h"
#include "yaCollisionManager.h"
#include "yaGround.h"
#include "yaHpUI.h"
#include "yaLife.h"
#include "yaObject.h"
#include "yaUIManager.h"
#include "yaHUD.h"
#include "yaPlayer_Save.h"
#include "yaApplication.h"
#include "yaSound.h"

#include "yaCollider.h"
#include "yaGameObject.h"
#include "yaObject.h"







namespace ya
{


	PlayScene::PlayScene()
		: m_Img(nullptr)
		, m_pPlayer(nullptr)
		, m_pHornetBgm(nullptr)
		, m_pFalseBgm(nullptr)
		, m_pBackBgm(nullptr)
	{
		if(nullptr == m_Img)
		m_Img = Resources::Load<Texture>(L"Map", L"..\\Resources\\Image\\Map.bmp");

		if(nullptr == m_pBackBgm)
		m_pBackBgm = Resources::Load<Sound>(L"BackGround1", L"..\\Resources\\Sound\\BackGround1.wav");
		if (nullptr == m_pHornetBgm)
		m_pHornetBgm = Resources::Load<Sound>(L"Hornet_Bgm", L"..\\Resources\\Sound\\Hornet_Bgm.wav");
		if (nullptr == m_pFalseBgm)
		m_pFalseBgm = Resources::Load<Sound>(L"False_Bgm", L"..\\Resources\\Sound\\False_Bgm.wav");

		

	}

	PlayScene::~PlayScene()
	{
	}

	void PlayScene::Initialize()
	{

		
		//if (MCIWndGetLength(m_hVideo) <= MCIWndGetPosition(m_hVideo))
		//{
		//	CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::GAME);
		//	return;
		//}

		//HUD* hud = UIManager::GetUiInstant<HUD>(eUIType::HP);
		//hud->ImageLoad(L"Hp", L"..\\Resources\\Image\\Hp.bmp");

		//HUD* hud = UIManager::GetUiInstant<HUD>(eUIType::MP);		



	}

	void PlayScene::Tick()
	{
		
		if (m_pPlayer->GetState().bgm == PLAYER_BGM::Back)
		{
			State Player = m_pPlayer->GetState();
			Player.bgm = PLAYER_BGM::None;
			m_pHornetBgm->Stop(true);
			m_pFalseBgm->Stop(true);
			m_pBackBgm->Play(true);
			m_pPlayer->SetState(Player);
			
		}
		if (m_pPlayer->GetState().bgm == PLAYER_BGM::False)
		{
			State Player = m_pPlayer->GetState();
			Player.bgm = PLAYER_BGM::None;
			m_pHornetBgm->Stop(true);
			m_pBackBgm->Stop(true);
			m_pFalseBgm->Play(true);
			m_pPlayer->SetState(Player);
		}
		if (m_pPlayer->GetState().bgm == PLAYER_BGM::Hornet)
		{
			State Player = m_pPlayer->GetState();
			Player.bgm = PLAYER_BGM::None;
			m_pBackBgm->Stop(true);
			m_pFalseBgm->Stop(true);
			m_pHornetBgm->Play(true);
			m_pPlayer->SetState(Player);
		}



		

		


		if (KEY_DOWN(eKeyCode::N))
		{
			SceneManager::ChangeScene(eSceneType::End);
		}

		Scene::Tick();
	}

	void PlayScene::Render(HDC hdc)
	{
		
		wchar_t szFloat[50] = {};
		swprintf_s(szFloat, 50, L"Play Scene");
		int strLen = (int)(wcsnlen_s(szFloat, 50));
		TextOut(hdc, 10, 30, szFloat, strLen);


		Vector2 Pos = Camera::CalculatePos(Vector2(0.f, 0.f));

		TransparentBlt(hdc
			, (int)Pos.x
			, (int)Pos.y
			, m_Img->GetWidth(), m_Img->GetHeight()
			, m_Img->GetDC(), 0, 0
			, m_Img->GetWidth(), m_Img->GetHeight(), RGB(255, 0, 255));

		Scene::Render(hdc);
	}

	void PlayScene::Enter()
	{

		Scene::Initialize();
		UIManager::Pop(eUIType::START);
		UIManager::Pop(eUIType::ANIMATIONPANEL);
		UIManager::Pop(eUIType::GROUNDBUTTON);
		UIManager::Push(eUIType::HP);

		Player_Save* save = ya::object::Instantiate<Player_Save>(eColliderLayer::GameObject);

		m_pPlayer = ya::object::Instantiate<Player>(eColliderLayer::Player);
		//m_pPlayer->SetPos(Vector2(115.f, 4291.f));
		//Camera::SetFalse(true);
		//m_pPlayer->SetPos(Vector2(2805.f, 4330.f));
		//Camera::SetHornet(true);

		m_pPlayer->SetPos(Vector2(1693.0f, 100.0f));
		m_pPlayer->SetResurrection(true);
		Camera::SetTarget(m_pPlayer);


		HpUI* hp = UIManager::GetUiInstant<HpUI>(eUIType::HP);
		hp->SetTarget(m_pPlayer);

		Life* life = UIManager::GetUiInstant<Life>(eUIType::LIFE);
		life->SetTarget(m_pPlayer);

		FILE* file = nullptr;
		Earth* LoadBuuton = new Earth;
		LoadBuuton->Monster_Load(file, m_pPlayer);
		
		
		Scene* playScene = SceneManager::GetPlayScene();
		playScene->AddGameObject(LoadBuuton, eColliderLayer::Ground);
		
		CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, true);
		CollisionManager::SetLayer(eColliderLayer::Monster_Projecttile, eColliderLayer::Player_Projecttile, true);
		CollisionManager::SetLayer(eColliderLayer::Ground, eColliderLayer::Player, true);
		CollisionManager::SetLayer(eColliderLayer::Ground, eColliderLayer::Monster, true);		
		CollisionManager::SetLayer(eColliderLayer::Monster_Projecttile, eColliderLayer::Player, true);
		CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, true);
		CollisionManager::SetLayer(eColliderLayer::Player, eColliderLayer::GameObject, true);
	}

	void PlayScene::Exit()
	{


	}
}
