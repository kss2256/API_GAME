#include "yaAnimationTool.h"
#include "yaApplication.h"
#include "yaUIManager.h"
#include "yaInput.h"
#include "yaGameObject.h"
#include "yaSceneManager.h"





namespace ya
{

	AnimationTool::AnimationTool()
		
	{


	}

	AnimationTool::~AnimationTool()
	{

	}

	void AnimationTool::Initialize()
	{

	}

	void AnimationTool::Tick()
	{
		if (KEY_DOWN(eKeyCode::ESC))
		{
			ya::SceneManager::ChangeScene(eSceneType::Logo);
		}








		Scene::Tick();

	}

	void AnimationTool::Render(HDC hdc)
	{

		


		Scene::Render(hdc);
	}

	void AnimationTool::Enter()
	{

		Scene::Initialize();
		Application::GetInstance().SetMenuBar(true);
		UIManager::Pop(eUIType::START);
		UIManager::Pop(eUIType::GROUNDBUTTON);
		UIManager::Pop(eUIType::HP);
		UIManager::Push(eUIType::ANIMATIONPANEL);
	}

	void AnimationTool::Exit()
	{



	}





}