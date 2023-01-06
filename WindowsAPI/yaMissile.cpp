#include "yaMissile.h"
#include "yaTime.h"
#include "yaCollider.h"
#include "yaCamera.h"
#include "yaAnimator.h"
#include "yaResources.h"
#include "yaImage.h"
#include "yaInput.h"

namespace ya
{
	Missile::Missile()
		: mSpeed(1.0f)
		, mAliveTime(30.0f)
	{
		SetPos({ 100.0f, 100.0f });
		SetScale({ 20.0f, 20.0f });

		Collider* col = new Collider();
		col->SetScale(Vector2(20.0f, 20.0f));

		AddComponent(col);
		if (mImage == nullptr)
		{
			mImage = Resources::Load<Texture>(L"msiile", L"..\\Resources\\Image\\msiile.bmp");
		}

		//Animator* ani = new Animator();
		//ani->CreateAnimation(L"Idle", mImage
		//	, Vector2(0.0f, 0.0f), Vector2(120.0f, 130.0f)
		//	, Vector2(5.0f, -20.0f), 3, 0.1f);

		//ani->Play(L"Idle", true);

		//AddComponent(ani);
		//Camera::SetTarget(this);

		mDir += Vector2(0.0f, -1.0f);
	}

	Missile::~Missile()
	{

	}

	void Missile::Tick()
	{
		GameObject::Tick();
		mAliveTime -= Time::DeltaTime();
		if (mAliveTime <= 0.0f)
		{
			this->Death();
		}

		float radian = math::DegreeToRadian(90.0f);
		float degree = math::RadianToDegree(2 * PI);
		float speed = 200.0f;

		Vector2 pos = GetPos();

		// 삼각함수를 이용한 회전
		//mDir = PI / 2.0f;
		//pos.x += speed * cosf(mDir) * Time::DeltaTime();
		//pos.y -= speed * sinf(mDir) * Time::DeltaTime();
		//Vector2 mousPos = Input::GetMousePos();
		//
		//mDir.Normailize();

		// 벡터를 이용한 회전
		pos.y += mDir.y * speed * Time::DeltaTime();
		pos.x += mDir.x * speed * Time::DeltaTime();

		SetPos(pos);
	}

	void Missile::Render(HDC hdc)
	{
		Vector2 pos = GetPos();
		Vector2 scale = GetScale();

		//Vector2 finalPos;
		//finalPos.x = (pos.x - mImage->GetWidth() * (scale.x / 0.0f));
		//finalPos.y = (pos.y - mImage->GetHeight() * (scale.y / 2.0f));

		Vector2 rect;
		rect.x = (float)mImage->GetWidth(); //*scale.x;
		rect.y = (float)mImage->GetHeight(); //*scale.y;

		pos = Camera::CalculatePos(pos);

		TransparentBlt(hdc
			, (int)pos.x
			, (int)pos.y
			, (int)rect.x
			, (int)rect.y
			, mImage->GetDC(), 0, 0, mImage->GetWidth(), mImage->GetHeight()
			, RGB(255, 0, 255));

		GameObject::Render(hdc);

	
	}
	void Missile::OnCollisionEnter(Collider* other)
	{
		GameObject* gameObj = other->GetOwner();
		gameObj->Death();

		this->Death();
	}

	void Missile::OnCollisionStay(Collider* other)
	{
	}
	void Missile::OnCollisionExit(Collider* other)
	{
	}
}