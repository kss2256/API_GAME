#pragma once

#include "yaGameObject.h"


namespace ya
{
	class Texture;
	class GroundEarth : public GameObject
	{

	public:
		GroundEarth();
		~GroundEarth();


	public:

		virtual void Tick();
		virtual void Render(HDC hdc);

		void SetGround(eGroundType _TYPE) { m_eGround = _TYPE; }
		eGroundType GetGround() { return m_eGround; }

	private:

		eGroundType			m_eGround;
		Texture*			m_Img;
		Texture*			m_pPlayer;
		Texture*			m_pGround;
		Texture*			m_pGround1;
		Texture*			m_pGround2;
		Texture*			m_pObject;
		Texture*			m_pObject1;
		Texture*			m_pObject2;
		Texture*			m_pLineCol;
		Texture*			m_pLineRol;
		Texture*			m_pRedLineCol;
		Texture*			m_pRedLineRol;
		Texture*			m_pBaldur;
		Texture*			m_pHusk;
		Texture*			m_pHornet;
		Texture*			m_pFalseKnight;


		Vector2			m_vStartPos;
		Vector2			m_vEndPos;
		Vector2			m_vMousePos;
		Vector2			m_vLineRender;
		Vector2			m_vLineMouse;
	

		bool			m_bRenderCheak;

	};

}


