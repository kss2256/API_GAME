#include "yaLineGround.h"
#include "yaPlayer.h"
#include "yaCollider.h"
#include "yaUIManager.h"
#include "yaRigidbody.h"
#include "yaCamera.h"


namespace ya
{
	LineGround::LineGround()
		: m_vStartPos(Vector2::Zero)
		, m_vEndPos(Vector2::Zero)
		, m_vStartrenderPos(Vector2::Zero)
		, m_vEndrenderPos(Vector2::Zero)
	{
		Collider* collider = AddComponent<Collider>();

		collider->SetPos(m_vStartrenderPos);
		collider->SetScale(m_vEndrenderPos);
	
		
	}

	LineGround::~LineGround()
	{

	}

	void LineGround::Tick()
	{



		GameObject::Tick();
	}

	void LineGround::Render(HDC hdc)
	{

		HPEN GreenPen = (HPEN)CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		HPEN OriginalPen = (HPEN)SelectObject(hdc, GreenPen);


		HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH hOriginBrush = (HBRUSH)SelectObject(hdc, hNullBrush);

		
		m_vStartrenderPos = m_vStartPos;
		m_vEndrenderPos = m_vEndPos;

		m_vStartrenderPos = Camera::CalculatePos(m_vStartrenderPos);
		m_vEndrenderPos = Camera::CalculatePos(m_vEndrenderPos);

		MoveToEx(hdc, (int)m_vStartrenderPos.x, (int)m_vStartrenderPos.y, nullptr);
		LineTo(hdc, (int)m_vEndrenderPos.x, (int)m_vStartrenderPos.y);

		GreenPen = (HPEN)SelectObject(hdc, OriginalPen);
		DeleteObject(GreenPen);
		SelectObject(hdc, hOriginBrush);


		GameObject::Render(hdc);
	}

	void LineGround::OnCollisionEnter(Collider* other)
	{
		Player* playerObj = dynamic_cast<Player*>(other->GetOwner());
		playerObj->GetComponent<Rigidbody>()->SetGround(true);

	}

	void LineGround::OnCollisionStay(Collider* other)
	{

	}

	void LineGround::OnCollisionExit(Collider* other)
	{

	}



}
