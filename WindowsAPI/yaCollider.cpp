#include "yaCollider.h"
#include "yaGameObject.h"
#include "yaApplication.h"
#include "yaCamera.h"

namespace ya
{
	bool Collider::m_bCheakRen = true;

	Collider::Collider()
		: Component(eComponentType::Collider)
		, mOffset(Vector2::Zero)
		, mPos(Vector2::Zero)
		, mScale(Vector2::One)
		, mCollisionCount(0)
		, m_LineRender(false)
	{
		mScale = Vector2(100.0f, 100.0f);
	}

	Collider::~Collider()
	{

	}

	void Collider::Tick()
	{
		GameObject* owner = GetOwner();

		mPos = owner->GetPos() + mOffset;
	}

	void Collider::Render(HDC hdc)
	{

		if (m_bCheakRen)
		{
			HBRUSH tr = Application::GetInstance().GetBrush(eBrushColor::Transparent);
			Brush brush(hdc, tr);

			HPEN greenPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
			HPEN redPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
			HPEN oldPen = NULL;

			if (mCollisionCount > 0)
				oldPen = (HPEN)SelectObject(hdc, redPen);
			else
				oldPen = (HPEN)SelectObject(hdc, greenPen);



			if (m_LineRender)
			{

				mPos = Camera::CalculatePos(mPos);

				//if (m_sDir == LineDir::Col)
				//{
				//	MoveToEx(hdc, (int)(mPos.x - mScale.x / 2), (int)(mPos.y - mScale.y / 2), nullptr);
				//	LineTo(hdc, (int)(mPos.x + mScale.x / 2), (int)(mPos.y - mScale.y / 2));
				//}
				//
				//if (m_sDir == LineDir::Rol)
				//{
				//	MoveToEx(hdc, (int)(mPos.x - mScale.x / 2), (int)(mPos.y - mScale.y / 2), nullptr);
				//	LineTo(hdc, (int)(mPos.x - mScale.x / 2), (int)(mPos.y + mScale.y / 2));
				//}

				Rectangle(hdc
					, (int)(mPos.x - mScale.x / 2.0f)
					, (int)(mPos.y - mScale.y / 2.0f)
					, (int)(mPos.x + mScale.x / 2.0f)
					, (int)(mPos.y + mScale.y / 2.0f));
			}
			else
			{
				mPos = Camera::CalculatePos(mPos);


				Rectangle(hdc
					, (int)(mPos.x - mScale.x / 2.0f)
					, (int)(mPos.y - mScale.y / 2.0f)
					, (int)(mPos.x + mScale.x / 2.0f)
					, (int)(mPos.y + mScale.y / 2.0f));
			}

			SelectObject(hdc, oldPen);
			DeleteObject(redPen);
			DeleteObject(greenPen);
		}
		else
		{

		}

	}

	void Collider::OnCollisionEnter(Collider* other)
	{
		mCollisionCount++;
		GetOwner()->OnCollisionEnter(other);
	}

	void Collider::OnCollisionStay(Collider* other)
	{
		GetOwner()->OnCollisionStay(other);
	}

	void Collider::OnCollisionExit(Collider* other)
	{
		mCollisionCount--;
		GetOwner()->OnCollisionExit(other);
	}
}
