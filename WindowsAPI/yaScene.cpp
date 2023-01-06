#include "yaScene.h"
#include "yaGameObject.h"
#include "yaSceneManager.h"
#include "yaCollisionManager.h"
#include "yaApplication.h"
#include "yaCamera.h"

namespace ya
{

	Scene::Scene()
		: mWindowInfo{}
	{
		SceneManager::SetPlayScene(this);
		mObjects.resize(_COLLIDER_LAYER);
		mWindowInfo = ya::Application::GetInstance().GetWindowData();
	}

	Scene::~Scene()
	{

		size_t a = mObjects.size();
		for (size_t y = 0; y < _COLLIDER_LAYER; y++)
		{
			for (size_t x = 0; x < mObjects[y].size(); x++)
			{
				delete mObjects[y][x];
				mObjects[y][x] = nullptr;
			}
		}
	}

	void Scene::Initialize()
	{
		
		

		for (size_t y = 0; y < _COLLIDER_LAYER; y++)
		{
			for (size_t x = 0; x < mObjects[y].size(); x++)
			{
				if (mObjects[y][x] == nullptr)
					continue;
				if (mObjects[y][x]->IsDeath())
					continue;

				mObjects[y][x]->Initialize();
			}
		}

	}



	void Scene::Tick()
	{
		for (size_t y = 0; y < _COLLIDER_LAYER; y++)
		{
			for (size_t x = 0; x < mObjects[y].size(); x++)
			{
				if (mObjects[y][x] == nullptr)
					continue;
				if (mObjects[y][x]->IsDeath())
					continue;
					mObjects[y][x]->Tick();
			}
		}
	}

	void Scene::Render(HDC hdc)
	{
		for (size_t y = 0; y < _COLLIDER_LAYER; y++)
		{
			for (size_t x = 0; x < mObjects[y].size(); x++)
			{
				if (mObjects[y][x] == nullptr)
					continue;
				if (mObjects[y][x]->IsDeath())
					continue;

				//Pos를 현재 실 좌표로 바꿔야하나..?
				//Vector2 Pos = mObjects[y][x]->GetPos();
			
				//if (Pos.x < -100 || Pos.y < -100)
				//	continue;
				//if (Pos.x > mWindowInfo.width + 100 || Pos.y > mWindowInfo.height + 100)
				//	continue;

				mObjects[y][x]->Render(hdc);
			}
		}

		// 이다음삭제를 해줘야한다.
	}

	void Scene::Enter()
	{

	}

	void Scene::Exit()
	{
		//CollisionManager::Clear();
	}

	void Scene::AddGameObject(GameObject* object, eColliderLayer type)
	{
		if (object == nullptr)
			return;


		mObjects[(UINT)type].push_back(object);
	}
}