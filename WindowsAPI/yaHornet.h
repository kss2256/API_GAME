#pragma once
#include "yaGameObject.h"

namespace ya
{
    enum class Cheak
    {
        None,
        Evade,
        Jump,
        Walk,
        Walk_End,
        End,
    };

    enum class HornetAction
    {
        None,       
        Damage,     
        Walk,       //이동
        Evade,      //백 스텝
        Jump,       //점프
        Play,       //페이즈 변경, 첫 등장씬
        G_Dash,     //땅 돌진 공격
        A_Dash,     //공중 돌진 공격
        Spear,      //창 던지기
        Counter,    //막기     
        G_Sphere,   //공중 구체
        A_Sphere,   //땅 구체
        Standing,   //서 있는 상태
        Wall,       //벽
        Stun,       //스턴
        Drop,       //떨어지는 도중
        Trap,       //플레이어 위치로 이동?
        Fail,
        Before_End, //끝내기 전
        End,
    };

    enum class HornetAI
    {
        None,           //랜덤 으로 상태 변경 해주기
        Phase,          //2단계로 변경 (추가로 스킬 늘어나게 하기?) (조건 충족시만 발동)
        G_Dash,         //    //    2
        A_Dash,         // 공 //    3
        Spear,          //    //    4
        Trap,           // 격 //    5
        G_Sphere,       //    //    6
        A_Sphere,       //    //    7    
        Defense,        //방어            (트랩 설치, 막기)
        Dumi,           //
        Play,           //
        Ending,         //
        Death,
        End,            //모든 행동의 마지막, 플레이어 위치 확인하고 스탠딩 상태or 이동 하기
    };

    struct HSTATE
    {

        DIRECTION           Direction;
        HornetAction        HAction;
        HornetAI            HIA;
        Cheak               TarGet;
    };

    
    
}



namespace ya
{
    class Sound;
    class Hornet_Attack;
    class Collider;
    class Texture;
    class Animator;
	class Hornet
		: public GameObject
	{
    private:

        int             m_Hp;

        static int      m_iPhase;
        static float    s_fTimer;      //몬스터 행동 딜레이
        static int      s_iCheak;      //랜덤 행동 같은행동 반복하면 자동으로 공격 하게
        static int      s_iRandom;      //랜덤
        static bool     s_bStart;
        
        Cheak           m_Chaek;

        HSTATE          m_tMonster;    //몬스터 행동 지시
        HSTATE          m_tHitSaveAi;  //피격시 이전 행동 저장

        Vector2         m_vTargetPos;  //플레이어 위치 저장
        Vector2         m_vFixPos;     //몬스터 소환 위치 저장(기준점)
        Vector2         m_vMovePos;    //몬스터 위치 
        Vector2         m_vAttackPos;  //몬스터 공격 범위

        Texture*        m_pHornet1Img;  //몬스터 이미지
        Texture*        m_pHornet2Img;  //몬스터 이미지
        Texture*        m_pHornet3Img;  //몬스터 이미지
        Texture*        m_pHornetEffectImg;  //몬스터 이미지
        Texture*        m_pHornetEndImg;  //몬스터 이미지
        Animator*       m_pHornetAni;  //몬스터 애니메이션
        Animator*       m_pHornetAniEffect;  //몬스터 이펙트 애니메이션
        GameObject*     m_pPlayer;     //플레이어 위치
        Collider*       m_pMonsterSize;	//몬스터 콜라이더 크기 조절용
        Hornet_Attack*  m_Attack;       //원거리 공격 용 

        Sound*          m_pStart;
        Sound*          m_pLeftEvade;
        Sound*          m_pRightEvade;
        Sound*          m_pWalk;
        Sound*          m_pG_Dash;
        Sound*          m_pDash;
        Sound*          m_pSphere;
        Sound*          m_pSphere1;
        Sound*          m_pA_Sphere;
        Sound*          m_pJump;
        Sound*          m_pS_Jump;
        Sound*          m_pJump_Effect;
        Sound*          m_pLand;
        Sound*          m_pSpear;
        Sound*          m_pSpear1;
        Sound*          m_pSpear2;
        Sound*          m_pTrap;
        Sound*          m_pTrap1;
        Sound*          m_pCounter;
        Sound*          m_pCounter1;
        Sound*          m_pStun;
        Sound*          m_pDeath;
        Sound*          m_pHit;



        bool            m_bTarget;     //몬스터 추적상태
        bool            m_bCounter;    //방어 성공 유무
        bool            m_bChange;     //페이즈 변경 유무
        bool            m_bPlay;     //페이즈 변경 유무
        bool            m_bHornet_Death;     

        bool	m_bLeftWalkCheak;
        bool	m_bRightWalkCheak;

    public:

        virtual void Tick() override;
        virtual void Render(HDC hdc) override;

        virtual void OnCollisionEnter(Collider* other) override;
        virtual void OnCollisionStay(Collider* other) override;
        virtual void OnCollisionExit(Collider* other) override;

        void SetTarget(GameObject* target) { m_pPlayer = target; }
        GameObject* GetTarget() { return m_pPlayer; }

        void SetBTar(bool target) { m_bTarget = target; }
        bool GetBTar() { return m_bTarget; }

        void SetHp(int hp) { m_Hp = hp; }
        int GetHp() { return m_Hp; }


        void SetLeftWalk(bool walk) { m_bLeftWalkCheak = walk; }
        bool GetLeftWalk() { return m_bLeftWalkCheak; }

        void SetRightWalk(bool walk) { m_bRightWalkCheak = walk; }
        bool GetRightWalk() { return m_bRightWalkCheak; }

        void SetWalk(HSTATE walk) { m_tMonster = walk; }
        HSTATE GetWalk() { return m_tMonster; }

    private:
        //이미지 애니메이션 업로드
        void AnimationInitialize();
        //Sound 업로드
        void SoundInitialize();

        //Hornet Action        
        void G_Dash_Attack();
        void Sphere_Attack();
        void Jump_Sphere_Attack();
        void Spear_Attack();

        void Hornet_Trap();
        void Hornet_Defence();
        void Hornet_Jump();
        void Evade();
        void Hornet_Stun();

        void Hornet_End();
        void Hornet_Play();
        void HornetWalk();
        void HornetWalk(float distance);
        void HornetPushWalk(float distance);
        

    public:
        Hornet();
        ~Hornet();




	};



}
