#pragma once

#include "yaGameObject.h"


namespace ya
{
	enum class MapType
	{
		Ground,
		Ground1,
		Ground2,
		Object,
		Object1,
		Object2,
		Col,
		Rol,
		Red_Col,
		Red_Rol,
		Baldur,
		Husk,
		Hornet,
		False_Knight,
		Player,

		END,
	};
	class GameObject;
	class Player;
	class Texture;
	class Earth
		: public GameObject
	{
	public:
		Earth();
		virtual ~Earth();

	public:

		virtual void Initialize() override;
		virtual void Tick() override;
		virtual void Render(HDC hdc) override;

		void Save(FILE* _file);
		void Load(FILE* _file);

		void Monster_Save(FILE* _file);
		void Monster_Load(FILE* _file, GameObject* _target);


		void SetLineStart(Vector2 start) { m_pLineStart = start; }
		void SetLineEnd(Vector2 end) { m_pLineEnd = end; }

		void SetGround(eGroundType _TYPE) { m_eGround = _TYPE; }
		Vector2 GetLineStart() { return m_pLineStart; }
		Vector2 GetLineEnd() { return m_pLineEnd; }
		eGroundType GetGround() { return m_eGround; }

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;

	private:

		eGroundType		m_eGround;
		MapType			m_eMapType;
		Texture*		m_Img;
		Texture*		m_pGround;
		Texture*		m_pGround1;
		Texture*		m_pGround2;
		Texture*		m_pObject;
		Texture*		m_pObject1;
		Texture*		m_pObject2;
		Texture*		m_pLine;





		
		Vector2			m_pLineStart;
		Vector2			m_pLineEnd;

		Player*			m_pPlayer;

	};


}

