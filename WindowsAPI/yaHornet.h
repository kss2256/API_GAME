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
        Walk,       //�̵�
        Evade,      //�� ����
        Jump,       //����
        Play,       //������ ����, ù �����
        G_Dash,     //�� ���� ����
        A_Dash,     //���� ���� ����
        Spear,      //â ������
        Counter,    //����     
        G_Sphere,   //���� ��ü
        A_Sphere,   //�� ��ü
        Standing,   //�� �ִ� ����
        Wall,       //��
        Stun,       //����
        Drop,       //�������� ����
        Trap,       //�÷��̾� ��ġ�� �̵�?
        Fail,
        Before_End, //������ ��
        End,
    };

    enum class HornetAI
    {
        None,           //���� ���� ���� ���� ���ֱ�
        Phase,          //2�ܰ�� ���� (�߰��� ��ų �þ�� �ϱ�?) (���� �����ø� �ߵ�)
        G_Dash,         //    //    2
        A_Dash,         // �� //    3
        Spear,          //    //    4
        Trap,           // �� //    5
        G_Sphere,       //    //    6
        A_Sphere,       //    //    7    
        Defense,        //���            (Ʈ�� ��ġ, ����)
        Dumi,           //
        Play,           //
        Ending,         //
        Death,
        End,            //��� �ൿ�� ������, �÷��̾� ��ġ Ȯ���ϰ� ���ĵ� ����or �̵� �ϱ�
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
        static float    s_fTimer;      //���� �ൿ ������
        static int      s_iCheak;      //���� �ൿ �����ൿ �ݺ��ϸ� �ڵ����� ���� �ϰ�
        static int      s_iRandom;      //����
        static bool     s_bStart;
        
        Cheak           m_Chaek;

        HSTATE          m_tMonster;    //���� �ൿ ����
        HSTATE          m_tHitSaveAi;  //�ǰݽ� ���� �ൿ ����

        Vector2         m_vTargetPos;  //�÷��̾� ��ġ ����
        Vector2         m_vFixPos;     //���� ��ȯ ��ġ ����(������)
        Vector2         m_vMovePos;    //���� ��ġ 
        Vector2         m_vAttackPos;  //���� ���� ����

        Texture*        m_pHornet1Img;  //���� �̹���
        Texture*        m_pHornet2Img;  //���� �̹���
        Texture*        m_pHornet3Img;  //���� �̹���
        Texture*        m_pHornetEffectImg;  //���� �̹���
        Texture*        m_pHornetEndImg;  //���� �̹���
        Animator*       m_pHornetAni;  //���� �ִϸ��̼�
        Animator*       m_pHornetAniEffect;  //���� ����Ʈ �ִϸ��̼�
        GameObject*     m_pPlayer;     //�÷��̾� ��ġ
        Collider*       m_pMonsterSize;	//���� �ݶ��̴� ũ�� ������
        Hornet_Attack*  m_Attack;       //���Ÿ� ���� �� 

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



        bool            m_bTarget;     //���� ��������
        bool            m_bCounter;    //��� ���� ����
        bool            m_bChange;     //������ ���� ����
        bool            m_bPlay;     //������ ���� ����
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
        //�̹��� �ִϸ��̼� ���ε�
        void AnimationInitialize();
        //Sound ���ε�
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
