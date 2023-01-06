#pragma once
#include "yaGameObject.h"


namespace ya
{
	class LineGround
		: public GameObject
	{

	private:
		Vector2 m_vStartPos;
		Vector2 m_vEndPos;
		Vector2 m_vStartrenderPos;
		Vector2 m_vEndrenderPos;


	public:
		//Get , Set ÇÔ¼ö
		Vector2 GetStartPos() { return m_vStartPos; }
		Vector2 GetEndPos() { return m_vEndPos; }

		void SetStartPos(Vector2 start) { m_vStartPos = start; }
		void SetEndPos(Vector2 end) { m_vEndPos = end; }



	public:


		virtual void Tick() override;
		virtual void Render(HDC hdc) override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;


	public:
		LineGround();
		~LineGround();



	};

}

