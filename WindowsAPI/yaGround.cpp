#include "yaGround.h"
#include "yaPlayer.h"
#include "yaCollider.h"
#include "yaRigidbody.h"
#include "yaHUD.h"
#include "yaUIManager.h"
#include "yaBaldur.h"
#include "yaHusk.h"
#include "yaHornet.h"
#include "yaFalse_Knight.h"

namespace ya
{
	Ground::Ground()
		: m_Cheak(false)
	{
		//Collider* collider = AddComponent<Collider>();
		
		//collider->SetScale(Vector2(600.0f, 50.0f));
	}

	Ground::~Ground()
	{
	}

	void Ground::Tick()
	{

		
		GameObject::Tick();
	}

	void Ground::Render(HDC hdc)
	{
		mPos = GetPos();
		mScale = GetScale();
		if(m_Cheak)
		{
			Collider* collider = AddComponent<Collider>();
			collider->SetPos(mPos);
			collider->SetScale(mScale);
			m_Cheak = false;
		}

		//MoveToEx(hdc, (int)(mPos.x - mScale.x / 2), (int)(mPos.y - mScale.y / 2), nullptr);
		//LineTo(hdc, (int)(mPos.x - mScale.x / 2), (int)(mPos.y + mScale.y / 2));


		GameObject::Render(hdc);
	}

	void Ground::OnCollisionEnter(Collider* other)
	{
		if (Player* playerObj = dynamic_cast<Player*>(other->GetOwner()))
		{	
			playerObj->GetPos();
			
			State player = playerObj->GetState();
			player.Pos;

			if (playerObj->GetPos().x > player.Pos.x)
			{
				//왼쪽에서 오른쪽 으로
				playerObj->SetRightWalk(true);
			}
			else
			{
				//오른쪽에서 왼쪽으로
				playerObj->SetLeftWalk(true);
			}				
		}
		if (Baldur* BaldurObj = dynamic_cast<Baldur*>(other->GetOwner()))
		{
			Command walk;
			walk = BaldurObj->GetWalk();
			if (walk.direction == DIRECTION::Left)
			{
				BaldurObj->SetLeftWalk(true);
			}
			if (walk.direction == DIRECTION::Right)
			{
				BaldurObj->SetRightWalk(true);
			}
		}
		if (Husk* HuskurObj = dynamic_cast<Husk*>(other->GetOwner()))
		{
			Command walk;
			walk = HuskurObj->GetWalk();
			if (walk.direction == DIRECTION::Left)
			{
				HuskurObj->SetLeftWalk(true);
			}
			if (walk.direction == DIRECTION::Right)
			{
				HuskurObj->SetRightWalk(true);
			}
		}

		if (Hornet* HornetObj = dynamic_cast<Hornet*>(other->GetOwner()))
		{
			HSTATE walk;
			walk = HornetObj->GetWalk();
			if (walk.Direction == DIRECTION::Left)
			{
				HornetObj->SetLeftWalk(true);
			}
			if (walk.Direction == DIRECTION::Right)
			{
				HornetObj->SetRightWalk(true);
			}
		}
		

	}

	void Ground::OnCollisionStay(Collider* other)
	{
		//if (Player* playerObj = dynamic_cast<Player*>(other->GetOwner()))
		//{
		//	playerObj->SetPos(mPos);
		// 
		//}
	

	}

	void Ground::OnCollisionExit(Collider* other)
	{
		if (Player* playerObj = dynamic_cast<Player*>(other->GetOwner()))
		{
			playerObj->SetRightWalk(false);
			playerObj->SetLeftWalk(false);
		}

		if (Baldur* BaldurObj = dynamic_cast<Baldur*>(other->GetOwner()))
		{
			BaldurObj->SetLeftWalk(false);
			BaldurObj->SetRightWalk(false);
		}

		if (Husk* HuskurObj = dynamic_cast<Husk*>(other->GetOwner()))
		{
			HuskurObj->SetLeftWalk(false);
			HuskurObj->SetRightWalk(false);
		}

		if (Hornet* HornetObj = dynamic_cast<Hornet*>(other->GetOwner()))
		{
			HornetObj->SetLeftWalk(false);
			HornetObj->SetRightWalk(false);
		}
	}

}