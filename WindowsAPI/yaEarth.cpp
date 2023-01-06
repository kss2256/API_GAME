#include "yaEarth.h"
#include "yaPlayer.h"
#include "yaCollider.h"
#include "yaRigidbody.h"
#include "yaHUD.h"
#include "yaGameObject.h"
#include "yaUIManager.h"
#include "yaImage.h"
#include "yaInput.h"
#include "yaCamera.h"
#include "func.h"
#include "yaSceneManager.h"
#include "yaScene.h"
#include "yaResources.h"
#include "yaGround.h"
#include "yaLineGround.h"
#include "yaHornet.h"
#include "yaHornet_Attack.h"
#include "yaBaldur.h"
#include "yaHusk.h"
#include "yaObject.h"
#include "yaGround.h"
#include "yaPlayer.h"
#include "yaFalse_Knight.h"
#include "yaFalse_Knight.h"
#include "yaFalse_Key.h"
#include "yaSound.h"
#include "yaSoundManager.h"


#include <commdlg.h>

namespace ya
{

	Earth::Earth()
		: m_eGround(eGroundType::END)
		, m_pLineStart(Vector2::Zero)
		, m_pLineEnd(Vector2::Zero)
		, m_Img(nullptr)
		, m_pPlayer(nullptr)
		, m_pGround(nullptr)
		, m_pGround1(nullptr)
		, m_pGround2(nullptr)
		, m_pObject(nullptr)
		, m_pObject1(nullptr)
		, m_pObject2(nullptr)
		, m_pLine(nullptr)
	{
		m_pGround = ya::Resources::Load<Texture>(L"Ground", L"..\\Resources\\Image\\Ground.bmp");
		m_pGround1 = ya::Resources::Load<Texture>(L"Ground1", L"..\\Resources\\Image\\Ground1.bmp");
		m_pGround2 = ya::Resources::Load<Texture>(L"Ground2", L"..\\Resources\\Image\\Ground2.bmp");
		m_pObject = ya::Resources::Load<Texture>(L"Object", L"..\\Resources\\Image\\Object.bmp");
		m_pObject1 = ya::Resources::Load<Texture>(L"Object1", L"..\\Resources\\Image\\Object1.bmp");
		m_pObject2 = ya::Resources::Load<Texture>(L"Object2", L"..\\Resources\\Image\\Object2.bmp");
		m_pLine = ya::Resources::Load<Texture>(L"dummy", L"..\\Resources\\Image\\dummy.bmp");


		//이거 진짜 질문 꼭꼭 해야 함...
		Texture* LineCol = Resources::Load<Texture>(L"Col", L"..\\Resources\\Image\\Col.bmp");
		Texture* LineRol = Resources::Load<Texture>(L"Rol", L"..\\Resources\\Image\\Rol.bmp");
		Texture* RedLineCol = Resources::Load<Texture>(L"Red_Col", L"..\\Resources\\Image\\Red_Col.bmp");
		Texture* RedLineRol = Resources::Load<Texture>(L"Red_Rol", L"..\\Resources\\Image\\Red_Rol.bmp");
		Texture* BaldurPrev = Resources::Load<Texture>(L"Baldur_Prev", L"..\\Resources\\Image\\Baldur_Prev.bmp");
		Texture* HuskPrev = Resources::Load<Texture>(L"Husk_Prev", L"..\\Resources\\Image\\Husk_Prev.bmp");
		Texture* HornetPrev = Resources::Load<Texture>(L"Hornet_Prev", L"..\\Resources\\Image\\Hornet_Prev.bmp");
		Texture* FalseKnightPrev = Resources::Load<Texture>(L"False_Knight_Prev", L"..\\Resources\\Image\\False_Knight_Prev.bmp");		
		Texture* GroundTool = Resources::Load<Texture>(L"GroundTool", L"..\\Resources\\Image\\GroundTool.bmp");
		Texture* Baldur = Resources::Load<Texture>(L"Baldur", L"..\\Resources\\Image\\Baldur.bmp");
		Texture* Wandering_Husk = Resources::Load<Texture>(L"Wandering_Husk", L"..\\Resources\\Image\\Wandering_Husk.bmp");
		Texture* Hornet1 = Resources::Load<Texture>(L"Hornet1", L"..\\Resources\\Image\\Hornet1.bmp");
		Texture* Hornet2 = Resources::Load<Texture>(L"Hornet2", L"..\\Resources\\Image\\Hornet2.bmp");
		Texture* Hornet3 = Resources::Load<Texture>(L"Hornet3", L"..\\Resources\\Image\\Hornet3.bmp");
		Texture* Hornet_Effect = Resources::Load<Texture>(L"Hornet_Effect", L"..\\Resources\\Image\\Hornet_Effect.bmp");
		Texture* Hornet_End = Resources::Load<Texture>(L"Hornet_End", L"..\\Resources\\Image\\Hornet_End.bmp");
		Texture* pStateImg = Resources::Load<Texture>(L"Player_State", L"..\\Resources\\Image\\Player_State.bmp");
		Texture* pPlayerAttackeffect = Resources::Load<Texture>(L"AttackEffect", L"..\\Resources\\Image\\AttackEffect.bmp");
		Texture* pPlayerAttack_Up_Down_effect = Resources::Load<Texture>(L"AttackEffect_Up_Down", L"..\\Resources\\Image\\AttackEffect_Up_Down.bmp");
		Texture* pPlayerDashEffect = Resources::Load<Texture>(L"Dash_Effect", L"..\\Resources\\Image\\Dash_Effect.bmp");
		Texture* pPlayerHitImg = Resources::Load<Texture>(L"Owner", L"..\\Resources\\Image\\Owner.bmp");
		Texture* pPlayerHit = Resources::Load<Texture>(L"Player_Hit", L"..\\Resources\\Image\\Player_Hit.bmp");
		Texture* False = Resources::Load<Texture>(L"False_Knight", L"..\\Resources\\Image\\False_Knight.bmp");
		Texture* Earthquake_Effect = Resources::Load<Texture>(L"Earthquake_Effect", L"..\\Resources\\Image\\Earthquake_Effect.bmp");
		Texture* PlayerMagic = Resources::Load<Texture>(L"Player_Magic", L"..\\Resources\\Image\\Player_Magic.bmp");
		Texture* Key = Resources::Load<Texture>(L"KeyImg", L"..\\Resources\\Image\\KeyImg.bmp");




		Sound * m_pSound = Resources::Load<Sound>(L"Monster_Damage_S", L"..\\Resources\\Sound\\Monster_Damage_S.wav");
		Sound* m_pSAttack = Resources::Load<Sound>(L"Player_sword_1", L"..\\Resources\\Sound\\Player_sword_1.wav");		
		Sound* m_pSMagic = Resources::Load<Sound>(L"Player_fireball", L"..\\Resources\\Sound\\Player_fireball.wav");
		Sound* m_pSDash = Resources::Load<Sound>(L"Player_Dash_S", L"..\\Resources\\Sound\\Player_Dash_S.wav");
		Sound* m_pSWalk = Resources::Load<Sound>(L"Player_Walk_S", L"..\\Resources\\Sound\\Player_Walk_S.wav");
		Sound* m_pSJump = Resources::Load<Sound>(L"Player_Jump_S", L"..\\Resources\\Sound\\Player_Jump_S.wav");
		Sound* m_pSFloating = Resources::Load<Sound>(L"Player_Floating_S", L"..\\Resources\\Sound\\Player_Floating_S.wav");
		Sound* m_pSHeal = Resources::Load<Sound>(L"Player_Heal_S", L"..\\Resources\\Sound\\Player_Heal_S.wav");
		Sound* m_pSHeal1 = Resources::Load<Sound>(L"Player_Heal_S1", L"..\\Resources\\Sound\\Player_Heal_S1.wav");
		Sound* m_pSLand = Resources::Load<Sound>(L"Player_Land_S", L"..\\Resources\\Sound\\Player_Land_S.wav");
		Sound* m_pSHit = Resources::Load<Sound>(L"Player_Hit_S", L"..\\Resources\\Sound\\Player_Hit_S.wav");
		Sound* m_pDeath = Resources::Load<Sound>(L"Player_Death_S", L"..\\Resources\\Sound\\Player_Death_S.wav");
		Sound* m_pStart = Resources::Load<Sound>(L"Hornet_Start_S", L"..\\Resources\\Sound\\Hornet_Start_S.wav");
		
		Sound* m_pLeftEvade = Resources::Load<Sound>(L"Hornet_Left_Laugh_S", L"..\\Resources\\Sound\\Hornet_Left_Laugh_S.wav");		
		Sound* m_pRightEvade = Resources::Load<Sound>(L"Hornet_Right_Laugh_S", L"..\\Resources\\Sound\\Hornet_Right_Laugh_S.wav");		
		Sound*	m_pWalk = Resources::Load<Sound>(L"hornet_Walk_S", L"..\\Resources\\Sound\\hornet_Walk_S.wav");
		Sound*	m_pG_Dash = Resources::Load<Sound>(L"Hornet_G_Dash_S", L"..\\Resources\\Sound\\Hornet_G_Dash_S.wav");
		Sound*	m_pDash = Resources::Load<Sound>(L"hornet_dash_S", L"..\\Resources\\Sound\\hornet_dash_S.wav");
		Sound*	m_pSphere = Resources::Load<Sound>(L"Hornet_Sphere_S", L"..\\Resources\\Sound\\Hornet_Sphere_S.wav");
		Sound*	m_pSphere1 = Resources::Load<Sound>(L"Hornet_Sphere_S1", L"..\\Resources\\Sound\\Hornet_Sphere_S1.wav");
		Sound*	m_pA_Sphere = Resources::Load<Sound>(L"Hornet_A_Sphere_S", L"..\\Resources\\Sound\\Hornet_A_Sphere_S.wav");
		Sound*	m_pJump = Resources::Load<Sound>(L"Hornet_Jump_S", L"..\\Resources\\Sound\\Hornet_Jump_S.wav");
		Sound*	m_pS_Jump = Resources::Load<Sound>(L"hornet_Small_Jump_S", L"..\\Resources\\Sound\\hornet_Small_Jump_S.wav");
		Sound*	m_pJump_Effect = Resources::Load<Sound>(L"hornet_jump_Effect_S", L"..\\Resources\\Sound\\hornet_jump_Effect_S.wav");
		Sound*	m_pLand = Resources::Load<Sound>(L"hornet_Land_S", L"..\\Resources\\Sound\\hornet_Land_S.wav");
		Sound*	m_pSpear = Resources::Load<Sound>(L"Hornet_Spear_S", L"..\\Resources\\Sound\\Hornet_Spear_S.wav");
		Sound*	m_pSpear1 = Resources::Load<Sound>(L"hornet_Spear_S1", L"..\\Resources\\Sound\\hornet_Spear_S1.wav");
		Sound*	m_pSpear2 = Resources::Load<Sound>(L"Hornet_Spear_S2", L"..\\Resources\\Sound\\hornet_Spear_S2.wav");
		Sound*	m_pTrap = Resources::Load<Sound>(L"hornet_Trap_S", L"..\\Resources\\Sound\\hornet_Trap_S.wav");
		Sound*	m_pTrap1 = Resources::Load<Sound>(L"hornet_Trap_S1", L"..\\Resources\\Sound\\hornet_Trap_S1.wav");
		Sound*	m_pCounter = Resources::Load<Sound>(L"hornet_Counter_S", L"..\\Resources\\Sound\\hornet_Counter_S.wav");
		Sound*	m_pCounter1 = Resources::Load<Sound>(L"hornet_Counter_S1", L"..\\Resources\\Sound\\hornet_Counter_S1.wav");
		Sound*	m_pStun = Resources::Load<Sound>(L"Hornet_Stun_S", L"..\\Resources\\Sound\\Hornet_Stun_S.wav");
		Sound*	ath = Resources::Load<Sound>(L"Hornet_Death_S", L"..\\Resources\\Sound\\Hornet_Death_S.wav");	
		Sound*	m_sShock = Resources::Load<Sound>(L"False_Shock_S", L"..\\Resources\\Sound\\False_Shock_S.wav");
		Sound*	m_sSwing = Resources::Load<Sound>(L"False_Swing_S", L"..\\Resources\\Sound\\False_Swing_S.wav");
		Sound*	m_sStrike = Resources::Load<Sound>(L"False_Strike_S", L"..\\Resources\\Sound\\False_Strike_S.wav");
		Sound*	m_sJump = Resources::Load<Sound>(L"False_Jump_S", L"..\\Resources\\Sound\\False_Jump_S.wav");
		Sound*	m_sLand = Resources::Load<Sound>(L"False_Land_S", L"..\\Resources\\Sound\\False_Land_S.wav");
		Sound*	m_sJump_Attack = Resources::Load<Sound>(L"False_Jump_Attack_S", L"..\\Resources\\Sound\\False_Jump_Attack_S.wav");
		Sound*	m_sShoulder = Resources::Load<Sound>(L"False_Shoulder_S", L"..\\Resources\\Sound\\False_Shoulder_S.wav");
		Sound*	m_sEarthquake = Resources::Load<Sound>(L"False_Earthquake_S", L"..\\Resources\\Sound\\False_Earthquake_S.wav");
		Sound*	m_sEarthquake1 = Resources::Load<Sound>(L"False_Earthquake_S1", L"..\\Resources\\Sound\\False_Earthquake_S1.wav");
		Sound*	m_sHit = Resources::Load<Sound>(L"False_Hit_S", L"..\\Resources\\Sound\\False_Hit_S.wav");
		Sound*	m_sStun = Resources::Load<Sound>(L"False_Stun_S", L"..\\Resources\\Sound\\False_Stun_S.wav");
		Sound*	m_sHead = Resources::Load<Sound>(L"False_Head_S", L"..\\Resources\\Sound\\False_Head_S.wav");
		Sound* m_pBackBgm = Resources::Load<Sound>(L"BackGround1", L"..\\Resources\\Sound\\BackGround1.wav");
		Sound* m_pHornetBgm = Resources::Load<Sound>(L"Hornet_Bgm", L"..\\Resources\\Sound\\Hornet_Bgm.wav");
		Sound* m_pFalseBgm = Resources::Load<Sound>(L"False_Bgm", L"..\\Resources\\Sound\\False_Bgm.wav");
		Sound* kye = Resources::Load<Sound>(L"stag_gate_open", L"..\\Resources\\Sound\\stag_gate_open.wav");


	}

	Earth::~Earth()
	{


	}



	void Earth::Initialize()
	{


		GameObject::Initialize();
	}
	 
	void Earth::Tick()
	{

		

		GameObject::Tick();
	}

	void Earth::Render(HDC hdc)
	{

		Vector2 Pos = Camera::CalculatePos(GetPos());


		switch (m_eGround)
		{
		case eGroundType::Ground:
		{

			Collider* coliider = new Collider();
			AddComponent(coliider);

			coliider->SetOffset(Vector2(0.f, -45.f));
			coliider->SetScale(Vector2(450.f, 50.f));

			m_Img = m_pGround;
			m_eMapType = MapType::Ground;
			m_eGround = eGroundType::END;
		}
		break;

		case eGroundType::Ground1:
		{
			Collider* coliider = new Collider();
			AddComponent(coliider);

			coliider->SetOffset(Vector2(0.f, -145.f));
			coliider->SetScale(Vector2(1820.f, 50.f));

			m_Img = m_pGround1;
			m_eMapType = MapType::Ground1;
			m_eGround = eGroundType::END;
		}
		break;

		case eGroundType::Ground2:
		{
			Collider* coliider = new Collider();
			AddComponent(coliider);

			coliider->SetOffset(Vector2(0.f, -43.f));
			coliider->SetScale(Vector2(144.f, 50.f));

			m_Img = m_pGround2;
			m_eMapType = MapType::Ground2;
			m_eGround = eGroundType::END;
		}
		break;

		case eGroundType::Object:
		{
			Collider* coliider = new Collider();
			AddComponent(coliider);

			coliider->SetOffset(Vector2(0.f, 0.f));
			coliider->SetScale(Vector2(100.f, 100.f));

			m_Img = m_pObject;
			m_eMapType = MapType::Object;
			m_eGround = eGroundType::END;
		}
		break;

		case eGroundType::Object1:
		{
			Collider* coliider = new Collider();
			AddComponent(coliider);

			coliider->SetOffset(Vector2(0.f, 6.f));
			coliider->SetScale(Vector2(140.f, 220.f));

			m_Img = m_pObject1;
			m_eMapType = MapType::Object1;
			m_eGround = eGroundType::END;
		}
		break;

		case eGroundType::Object2:
		{
			Collider* coliider = new Collider();
			AddComponent(coliider);

			coliider->SetOffset(Vector2(0.f, 30.f));
			coliider->SetScale(Vector2(180.f, 20.f));

			m_Img = m_pObject2;
			m_eMapType = MapType::Object2;
			m_eGround = eGroundType::END;
		}
		break;
		case eGroundType::Col:
		{
			Collider* coliider = new Collider();
			AddComponent(coliider);


			coliider->SetPos(GetPos());
			coliider->SetScale(Vector2(abs(GetLineStart().x - GetLineEnd().x), 50));
			coliider->SetLine(true);
			coliider->SetLineDir(LineDir::Col);
			m_Img = m_pLine;
			m_eMapType = MapType::Col;
			m_eGround = eGroundType::END;
		}
		break;
		case eGroundType::Rol:
		{
			Collider* coliider = new Collider();
			AddComponent(coliider);


			coliider->SetPos(GetPos());
			coliider->SetScale(Vector2(50, abs(GetLineStart().y - GetLineEnd().y)));
			coliider->SetLine(true);
			coliider->SetLineDir(LineDir::Rol);
			m_Img = m_pLine;
			m_eMapType = MapType::Rol;
			m_eGround = eGroundType::END;
		}
		break;
		case eGroundType::Red_Col:
		{

		}
		break;
		case eGroundType::Red_Rol:
		{
			Ground* baldur = ya::object::Instantiate<Ground>(eColliderLayer::Ground);
			baldur->SetPos(GetPos());
			baldur->SetPos(GetPos());
			baldur->SetScale(Vector2(1, abs(GetLineStart().y - GetLineEnd().y)));
			baldur->SetCheak(true);
			
			m_Img = m_pLine;
			m_eMapType = MapType::Red_Rol;
			m_eGround = eGroundType::END;
		}
		break;
		case eGroundType::Baldur:
		{
			Scene* playScene = SceneManager::GetPlayScene();

			std::vector<GameObject*>& gorund = playScene->GetGameObjects(eColliderLayer::Player);
			Baldur* baldur = ya::object::Instantiate<Baldur>(eColliderLayer::Monster);
			baldur->SetPos(GetPos());

			if (!(gorund.size() == 0))
			{
				for (size_t i = 0; gorund.size(); ++i)
				{
					if (gorund[i]->GetName() == L"Player")
					{
						baldur->SetTarget((Player*)gorund[i]);
						break;
					}
				}
			}
			else
			{
				baldur->SetBTar(false);
			}
			m_eMapType = MapType::Baldur;
			m_eGround = eGroundType::END;
			this->Death();
		}
		break;
		case eGroundType::Husk:
		{
			Scene* playScene = SceneManager::GetPlayScene();

			std::vector<GameObject*>& gorund = playScene->GetGameObjects(eColliderLayer::Player);

			Husk* husk = ya::object::Instantiate<Husk>(eColliderLayer::Monster);
			husk->SetPos(GetPos());
			if (!(gorund.size() == 0))
			{
				for (size_t i = 0; gorund.size(); ++i)
				{
					if (gorund[i]->GetName() == L"Player")
					{
						husk->SetTarget((Player*)gorund[i]);
						break;
					}
				}
			}
			else
			{
				husk->SetBTar(false);
			}

			m_eMapType = MapType::Husk;
			m_eGround = eGroundType::END;
			this->Death();
		}
		break;
		case eGroundType::Hornet:
		{
			Scene* playScene = SceneManager::GetPlayScene();

			std::vector<GameObject*>& gorund = playScene->GetGameObjects(eColliderLayer::Player);

			Hornet* hornet = ya::object::Instantiate<Hornet>(eColliderLayer::Monster);
			hornet->SetPos(GetPos());
			if (!(gorund.size() == 0))
			{
				for (size_t i = 0; gorund.size(); ++i)
				{
					if (gorund[i]->GetName() == L"Player")
					{
						hornet->SetTarget((Player*)gorund[i]);
						break;
					}
				}
			}
			else
			{
				hornet->SetBTar(false);
			}

			m_eMapType = MapType::Hornet;
			m_eGround = eGroundType::END;
			this->Death();
		}
		break;
		case eGroundType::False_Knight:
		{
			Scene* playScene = SceneManager::GetPlayScene();

			std::vector<GameObject*>& gorund = playScene->GetGameObjects(eColliderLayer::Player);

			False_Knight* false_knight = ya::object::Instantiate<False_Knight>(eColliderLayer::Monster);
			false_knight->SetPos(GetPos());
			if (!(gorund.size() == 0))
			{
				for (size_t i = 0; gorund.size(); ++i)
				{
					if (gorund[i]->GetName() == L"Player")
					{
						false_knight->SetTarget((Player*)gorund[i]);
						break;
					}
				}
			}
			else
			{
				false_knight->SetBTar(false);
			}

			m_eMapType = MapType::False_Knight;
			m_eGround = eGroundType::END;
			this->Death();
		}
		break;
		case eGroundType::Player:
		{
			Player* character = ya::object::Instantiate<Player>(eColliderLayer::Player);
			character->SetPos(GetPos());
			character->SetResurrection(true);
			Camera::SetTarget(character);

			m_eMapType = MapType::Player;
			m_eGround = eGroundType::END;


		}
		break;
		}

		BLENDFUNCTION tFunc = {};
		tFunc.BlendOp = AC_SRC_OVER;
		tFunc.BlendFlags = 0;
		tFunc.AlphaFormat = AC_SRC_ALPHA;
		tFunc.SourceConstantAlpha = 255;


		if (nullptr != m_Img)
		{
			AlphaBlend(hdc
				, (int)Pos.x - m_Img->GetWidth() / 2
				, (int)Pos.y - m_Img->GetHeight() / 2
				, m_Img->GetWidth(), m_Img->GetHeight()
				, m_Img->GetDC(), 0, 0
				, m_Img->GetWidth(), m_Img->GetHeight(), tFunc);
		}



		GameObject::Render(hdc);
	}

	void Earth::Save(FILE* _file)
	{

		// open a file name
		OPENFILENAME ofn = {};

		std::wstring PATH = L"..\\Resources\\TileSaveFiles\\";


		wchar_t szFilePath[256] = {};

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFilePath;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = L"0ALL\0*.*";//L"Tile\0*.tile\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = PATH.c_str();//NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (false == GetSaveFileName(&ofn))
			return;

		_file = nullptr;
		_wfopen_s(&_file, szFilePath, L"wb");
		if (_file == nullptr)
			return;

		//std::unordered_map<UINT64, Tile*>::iterator iter = mTiles.begin();
		//for (; iter != mTiles.end(); ++iter)
		//{
		//	int tileIndex = (*iter).second->GetIndex();
		//	fwrite(&tileIndex, sizeof(int), 1, pFile);

		//	TileID id;
		//	id.ID = (*iter).first;
		//	fwrite(&id.ID, sizeof(UINT64), 1, pFile);
		//}

		Scene* playScene = SceneManager::GetPlayScene();
		std::vector<GameObject*>& gorund = playScene->GetGameObjects(eColliderLayer::Ground);

		




		size_t tSize = gorund.size();

		if (_file == nullptr)
			return;

		
		fwrite(&tSize, sizeof(size_t), 1, _file);

		for (size_t i = 0; i < gorund.size(); ++i)
		{

			if (gorund[i]->IsDeath())
				continue;


			//위치 정보 저장
			Vector2 vPos = gorund[i]->GetPos();
			fwrite(&vPos, sizeof(Vector2), 1, _file);

			
			bool bAtlas = ((Earth*)gorund[i])->m_Img;
			fwrite(&bAtlas, sizeof(bool), 1, _file);

			if (bAtlas)
			{
				// 키 값 저장.
				std::wstring strkey = ((Earth*)gorund[i])->m_Img->GetKey();
				SaveWString(strkey, _file);

				//상대경로 저장
				std::wstring strRelativePath = ((Earth*)gorund[i])->m_Img->GetPath();
				SaveWString(strRelativePath, _file);
			}

			MapType ground = ((Earth*)gorund[i])->m_eMapType;
			fwrite(&ground, sizeof(MapType), 1, _file);
			

			Collider* leftCollider = gorund[i]->GetComponent<Collider>();
			
			if(leftCollider != nullptr)
			{
				bool Line = leftCollider->GetLine();
				fwrite(&Line, sizeof(bool), 1, _file);

				if(Line)
				{
					Vector2 collPos = leftCollider->GetPos();
					fwrite(&collPos, sizeof(Vector2), 1, _file);

					Vector2 collScale = leftCollider->GetScale();
					fwrite(&collScale, sizeof(Vector2), 1, _file);

					LineDir dir = leftCollider->GetLineDir();
					fwrite(&dir, sizeof(LineDir), 1, _file);
				}

			}


			

		}


		//몬스터 정보 저장하기
		Scene* Scene = SceneManager::GetPlayScene();
		std::vector<GameObject*>& Monster = Scene->GetGameObjects(eColliderLayer::Monster);
		
		if (Monster.size() != 0)
		{

			size_t mSize = Monster.size();

			if (_file == nullptr)
				return;


			fwrite(&mSize, sizeof(size_t), 1, _file);



			for (size_t i = 0; i < Monster.size(); ++i)
			{

				if (Monster[i]->IsDeath())
					continue;


				//위치 정보 저장
				Vector2 vPos = Monster[i]->GetPos();
				fwrite(&vPos, sizeof(Vector2), 1, _file);
								


				std::wstring name = Monster[i]->GetName();
				SaveWString(name, _file);

			


			}
		}


		fclose(_file);

	}

	void Earth::Load(FILE* _file)
	{
		OPENFILENAME ofn = {};

		wchar_t szFilePath[256] = {};

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFilePath;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (false == GetOpenFileName(&ofn))
			return;

		////이걸로 읽으면 바로 게임 실행 
		//wchar_t szFilePath[256] = L"..\\Resources\\TileSaveFiles\\TEST";


		_file = nullptr;
		_wfopen_s(&_file, szFilePath, L"rb");
		if (_file == nullptr)
			return;

		size_t iSize = 0;
		fread(&iSize, sizeof(size_t), 1, _file);

		Scene* playScene = SceneManager::GetPlayScene();
		std::vector<GameObject*>& gorund = playScene->GetGameObjects(eColliderLayer::Ground);


		for (int i = 0; i < iSize; ++i)
		{

			Vector2 vPos;
			fread(&vPos, sizeof(Vector2), 1, _file);

			SetPos(vPos);

			bool bAtlas = false;
			fread(&bAtlas, sizeof(bool), 1, _file);

			if (bAtlas)
			{
				std::wstring strkey, strRelativePath;
				LoadWString(strkey, _file);
				LoadWString(strRelativePath, _file);

				m_Img = Resources::Load<Texture>(strkey, strRelativePath); 

			
				eGroundType ground;
				fread(&ground, sizeof(MapType), 1, _file);


				bool Line;
				fread(&Line, sizeof(bool), 1, _file);
				

				Earth* pground = new Earth;
				pground->SetGround(ground);
				pground->SetPos(vPos);
				Scene* playScene = SceneManager::GetPlayScene();

				if(Line)
				{
					Vector2 collPos = {};
					fread(&collPos, sizeof(Vector2), 1, _file);


					Vector2 collScale = {};
					fread(&collScale, sizeof(Vector2), 1, _file);

					LineDir dir = {};
					fread(&dir, sizeof(LineDir), 1, _file);

					Collider* coliider = new Collider();
					pground->AddComponent(coliider);
					coliider->SetPos(collPos);
					coliider->SetScale(collScale);
					coliider->SetLine(Line);
					coliider->SetLineDir(dir);
				}		

				playScene->AddGameObject(pground, eColliderLayer::Ground);

				
			}
		}

		//몬스터 정보 불러오기
		Scene* Scene = SceneManager::GetPlayScene();
		std::vector<GameObject*>& Monster = Scene->GetGameObjects(eColliderLayer::Monster);
			

			if (_file == nullptr)
				return;

			size_t mSize = 0;			
			fread(&mSize, sizeof(size_t), 1, _file);



			for (size_t i = 0; i < mSize; ++i)
			{
			

				//위치 정보 불러오기
				Vector2 vmPos;
				fread(&vmPos, sizeof(Vector2), 1, _file);

				

				std::wstring strname;
				LoadWString(strname, _file);
				//fread(&strkey, sizeof(MapType), 1, _file);

					if (strname == L"Baldur")
					{
						Baldur* baldur = ya::object::Instantiate<Baldur>(eColliderLayer::Monster);
						baldur->SetPos(vmPos);						
					}
					if (strname == L"Hornet")
					{
						Hornet* hornet = ya::object::Instantiate<Hornet>(eColliderLayer::Monster);
						hornet->SetPos(vmPos);


					}
					if (strname == L"Husk")
					{
						Husk* husk = ya::object::Instantiate<Husk>(eColliderLayer::Monster);
						husk->SetPos(vmPos);


					}
					if (strname == L"False_Knight")
					{
						False_Knight* false_knight = ya::object::Instantiate<False_Knight>(eColliderLayer::Monster);
						false_knight->SetPos(vmPos);
										
					}
				
				
			}
	
		
		fclose(_file);

	}

	void Earth::Monster_Save(FILE* _file)
	{




	}

	void Earth::Monster_Load(FILE* _file, GameObject* _target)
	{
		//OPENFILENAME ofn = {};

		//wchar_t szFilePath[256] = {};

		//ZeroMemory(&ofn, sizeof(ofn));
		//ofn.lStructSize = sizeof(ofn);
		//ofn.hwndOwner = NULL;
		//ofn.lpstrFile = szFilePath;
		//ofn.lpstrFile[0] = '\0';
		//ofn.nMaxFile = 256;
		//ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
		//ofn.nFilterIndex = 1;
		//ofn.lpstrFileTitle = NULL;
		//ofn.nMaxFileTitle = 0;
		//ofn.lpstrInitialDir = NULL;
		//ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		//if (false == GetOpenFileName(&ofn))
		//	return;

		////이걸로 읽으면 바로 게임 실행 
		wchar_t szFilePath[256] = L"..\\Resources\\TileSaveFiles\\Game1";


		_file = nullptr;
		_wfopen_s(&_file, szFilePath, L"rb");
		if (_file == nullptr)
			return;

		size_t iSize = 0;
		fread(&iSize, sizeof(size_t), 1, _file);

		Scene* playScene = SceneManager::GetPlayScene();
		std::vector<GameObject*>& gorund = playScene->GetGameObjects(eColliderLayer::Ground);


		for (int i = 0; i < iSize; ++i)
		{

			Vector2 vPos;
			fread(&vPos, sizeof(Vector2), 1, _file);

			SetPos(vPos);

			bool bAtlas = false;
			fread(&bAtlas, sizeof(bool), 1, _file);

			if (bAtlas)
			{
				std::wstring strkey, strRelativePath;
				LoadWString(strkey, _file);
				LoadWString(strRelativePath, _file);

				m_Img = Resources::Load<Texture>(strkey, strRelativePath);


				eGroundType ground;
				fread(&ground, sizeof(MapType), 1, _file);


				bool Line;
				fread(&Line, sizeof(bool), 1, _file);


				Earth* pground = new Earth;
				pground->SetGround(ground);
				pground->SetPos(vPos);
				Scene* playScene = SceneManager::GetPlayScene();

				if (Line)
				{
					Vector2 collPos = {};
					fread(&collPos, sizeof(Vector2), 1, _file);


					Vector2 collScale = {};
					fread(&collScale, sizeof(Vector2), 1, _file);

					LineDir dir = {};
					fread(&dir, sizeof(LineDir), 1, _file);

					Collider* coliider = new Collider();
					pground->AddComponent(coliider);
					coliider->SetPos(collPos);
					coliider->SetScale(collScale);
					coliider->SetLine(Line);
					coliider->SetLineDir(dir);

				}

				playScene->AddGameObject(pground, eColliderLayer::Ground);


			}
		}

		//몬스터 정보 불러오기
		Scene* Scene = SceneManager::GetPlayScene();
		std::vector<GameObject*>& Monster = Scene->GetGameObjects(eColliderLayer::Monster);


		if (_file == nullptr)
			return;

		size_t mSize = 0;
		fread(&mSize, sizeof(size_t), 1, _file);



		for (size_t i = 0; i < mSize; ++i)
		{


			//위치 정보 불러오기
			Vector2 vmPos;
			fread(&vmPos, sizeof(Vector2), 1, _file);



			std::wstring strname;
			LoadWString(strname, _file);
			//fread(&strkey, sizeof(MapType), 1, _file);

			if (strname == L"Baldur")
			{
				Baldur* baldur = ya::object::Instantiate<Baldur>(eColliderLayer::Monster);
				baldur->SetPos(vmPos);
				if (_target != nullptr)
					baldur->SetTarget(_target);
				
			}
			if (strname == L"Hornet")
			{
				Hornet* hornet = ya::object::Instantiate<Hornet>(eColliderLayer::Monster);
				hornet->SetPos(vmPos);
				if (_target != nullptr)
					hornet->SetTarget(_target);

			}
			if (strname == L"Husk")
			{
				Husk* husk = ya::object::Instantiate<Husk>(eColliderLayer::Monster);
				husk->SetPos(vmPos);
				if (_target != nullptr)
					husk->SetTarget(_target);

			}
			if (strname == L"False_Knight")
			{
				//Husk* husk = ya::object::Instantiate<Husk>(eColliderLayer::Monster);
				//husk->SetPos(vmPos);
				//				
			}


		}


		fclose(_file);

	}


	void Earth::OnCollisionEnter(Collider* other)
	{

		
		if(Player* playerObj = dynamic_cast<Player*>(other->GetOwner()))
		{
			if (m_eMapType == MapType::Rol)
			{
				playerObj->GetPos();

				State player = playerObj->GetState();
				player.Pos;

				if (playerObj->GetPos().x > player.Pos.x)
				{
					//왼쪽에서 오른쪽 으로
					playerObj->SetRightWalk(true);
					return;
				}
				else
				{
					//오른쪽에서 왼쪽으로
					playerObj->SetLeftWalk(true);
					return;
				}
			}

			float fLen = fabs(other->GetPos().y - GetComponent<Collider>()->GetPos().y);
			float fScale = other->GetScale().y / 2.0f + GetComponent<Collider>()->GetScale().y / 2.0f;

			if (fLen < fScale)
			{
				Vector2 playerPos = playerObj->GetPos();
				playerPos.y -= (fScale - fLen) - 1.0f;
				playerObj->SetPos(playerPos);
			}
			playerObj->GetComponent<Rigidbody>()->SetGround(true);
		}

		if(Hornet* hornet = dynamic_cast<Hornet*>(other->GetOwner()))
		{
			if (m_eMapType == MapType::Rol)
			{
				
				HSTATE walk;
				walk = hornet->GetWalk();
				if (walk.Direction == DIRECTION::Left)
				{
					hornet->SetLeftWalk(true);
				}
				if (walk.Direction == DIRECTION::Right)
				{
					hornet->SetRightWalk(true);
				}
				return;
			}

			float fLen = fabs(other->GetPos().y - GetComponent<Collider>()->GetPos().y);
			float fScale = other->GetScale().y / 2.0f + GetComponent<Collider>()->GetScale().y / 2.0f;

			if (fLen < fScale)
			{
				Vector2 playerPos = hornet->GetPos();
				playerPos.y -= (fScale - fLen) - 1.0f;
				hornet->SetPos(playerPos);
			}

			hornet->GetComponent<Rigidbody>()->SetGround(true);
		}
		
		if (Baldur* baldur = dynamic_cast<Baldur*>(other->GetOwner()))
		{
			if (m_eMapType == MapType::Rol)
			{
				Command walk;
				walk = baldur->GetWalk();
				if (walk.direction == DIRECTION::Left)
				{
					baldur->SetLeftWalk(true);
				}
				if (walk.direction == DIRECTION::Right)
				{
					baldur->SetRightWalk(true);
				}
				return;
			}
			baldur->GetComponent<Rigidbody>()->SetGround(true);
		}

		if (Husk* husk = dynamic_cast<Husk*>(other->GetOwner()))
		{
			if (m_eMapType == MapType::Rol)
			{
				Command walk;
				walk = husk->GetWalk();
				if (walk.direction == DIRECTION::Left)
				{
					husk->SetLeftWalk(true);
				}
				if (walk.direction == DIRECTION::Right)
				{
					husk->SetRightWalk(true);
				}
				return;
			}
			husk->GetComponent<Rigidbody>()->SetGround(true);
		}

		if (False_Knight* false_knight = dynamic_cast<False_Knight*>(other->GetOwner()))
		{

			if (m_eMapType == MapType::Rol)
			{

				FALSE_AI walk;
				walk = false_knight->GetWalk();
				if (walk.Direction == DIRECTION::Left)
				{
					false_knight->SetLeftWalk(true);
				}
				if (walk.Direction == DIRECTION::Right)
				{
					false_knight->SetRightWalk(true);
				}
				return;
			}

			float fLen = fabs(other->GetPos().y - GetComponent<Collider>()->GetPos().y);
			float fScale = other->GetScale().y / 2.0f + GetComponent<Collider>()->GetScale().y / 2.0f;

			if (fLen < fScale)
			{
				Vector2 playerPos = false_knight->GetPos();
				playerPos.y -= (fScale - fLen) - 1.0f;
				false_knight->SetPos(playerPos);
			}

			false_knight->GetComponent<Rigidbody>()->SetGround(true);
		}

	}

	void Earth::OnCollisionStay(Collider* other)
	{
		

	}

	void Earth::OnCollisionExit(Collider* other)
	{
		if (Player* playerObj = dynamic_cast<Player*>(other->GetOwner()))
		{
			if (m_eMapType == MapType::Rol)
			{
				playerObj->SetRightWalk(false);
				playerObj->SetLeftWalk(false);
				return;
			}			
			
			playerObj->GetComponent<Rigidbody>()->SetGround(false);
		}

		if (Hornet* hornet = dynamic_cast<Hornet*>(other->GetOwner()))
		{			
			if (m_eMapType == MapType::Rol)
			{
				hornet->SetRightWalk(false);
				hornet->SetLeftWalk(false);
				return;
			}
			hornet->GetComponent<Rigidbody>()->SetGround(false);
		}

		if (Baldur* baldur = dynamic_cast<Baldur*>(other->GetOwner()))
		{
			if (m_eMapType == MapType::Rol)
			{
				baldur->SetRightWalk(false);
				baldur->SetLeftWalk(false);
				return;
			}
			baldur->GetComponent<Rigidbody>()->SetGround(false);
		}

		if (Husk* husk = dynamic_cast<Husk*>(other->GetOwner()))
		{
			if (m_eMapType == MapType::Rol)
			{
				husk->SetRightWalk(false);
				husk->SetLeftWalk(false);
				return;
			}
			husk->GetComponent<Rigidbody>()->SetGround(false);
		}

		if (False_Knight* false_knight = dynamic_cast<False_Knight*>(other->GetOwner()))
		{
			if (m_eMapType == MapType::Rol)
			{
				false_knight->SetRightWalk(false);
				false_knight->SetLeftWalk(false);
				return;
			}

			false_knight->GetComponent<Rigidbody>()->SetGround(false);
		}



	}



}
