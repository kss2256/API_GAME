#pragma once
#include "yaGameObject.h"

namespace ya
{
	class False_Potar
		: public GameObject
	{

	private:

		static float	s_fTimer;
		bool			m_bCheak;
		bool			m_bEnd;

			

	public:

		virtual void Tick() override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;


	public:
		False_Potar();
		~False_Potar();



	};





}


