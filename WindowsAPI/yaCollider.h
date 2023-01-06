#pragma once
#include "yaComponent.h"

enum class LineDir
{
	Col,
	Rol,
};


namespace ya
{
	class Collider : public Component
	{
	public:
		Collider();
		virtual ~Collider();

		virtual void Tick() override;
		virtual void Render(HDC hdc) override;

		virtual void OnCollisionEnter(Collider* other);
		virtual void OnCollisionStay(Collider* other);
		virtual void OnCollisionExit(Collider* other);

		virtual void SetScale(Vector2 scale) { mScale = scale; }
		virtual void SetPos(Vector2 pos) { mPos = pos; }
		virtual void SetOffset(Vector2 pos) { mOffset = pos; }
		virtual void SetLine(bool line) { m_LineRender = line; }
		virtual void SetLineDir(LineDir dir) { m_sDir = dir; }

		virtual Vector2 GetPos() { return mPos; }
		virtual Vector2 GetOffset() { return mOffset; }
		virtual Vector2 GetScale() { return mScale; }
		virtual bool GetLine() { return m_LineRender; }
		virtual LineDir GetLineDir() { return m_sDir; }
		virtual void SetCheakRen(bool cheak) { m_bCheakRen = cheak; }
		virtual bool GetCheakRen() { return m_bCheakRen; }

	private:
		Vector2 mOffset;
		Vector2 mPos;
		Vector2 mScale;
	
		bool	m_LineRender;
		LineDir		m_sDir;

		static bool	m_bCheakRen;

		UINT mCollisionCount;
	};
}
