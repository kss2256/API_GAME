#include "yaHusk.h"
#include "yaImage.h"
#include "yaTime.h"
#include "yaResources.h"
#include "yaAnimator.h"
#include "yaCollider.h"
#include "yaPlayer.h"
#include "yaRigidbody.h"
#include "yaPlayer_Attack.h"
#include "yaCollisionManager.h"
#include "yaHornet_Key.h"
#include "yaObject.h"
#include "yaSound.h"


namespace ya
{

    float Husk::s_fTimer = 0.f;

    Husk::Husk()
        : m_pHuskImg(nullptr)
        , m_pHuskAni(nullptr)
        , m_pPlayer(nullptr)
        , m_pSound(nullptr)
        , m_bTarget(false)
        , m_bTar(false)
        , m_bLeftWalkCheak(false)
        , m_bRightWalkCheak(false)
        , m_Hp(3)
        , m_vTargetPos(Vector2::Zero)
        , m_vFixPos(Vector2::Zero)
        , m_bFix(false)

    {
        SetName(L"Husk");
        SetPos({ 900.0f, 500.0f });
        SetScale({ 1.0f, 1.0f });

        m_pHuskImg = Resources::Load<Texture>(L"Wandering_Husk", L"..\\Resources\\Image\\Wandering_Husk.bmp");

        m_tMonster.action = ACTION::End;
        m_tMonster.direction = DIRECTION::Right;

        m_pHuskAni = new Animator();

        Collider* coliider = new Collider();
        AddComponent(coliider);
        coliider->SetScale(Vector2(110.0f, 140.0f));
        
        if (nullptr == m_pSound)
           m_pSound = Resources::Load<Sound>(L"Monster_Damage_S", L"..\\Resources\\Sound\\Monster_Damage_S.wav");

     
        AnimationInitialize();



        m_pHuskAni->Play(L"Husk_Right_Standing", true);


        AddComponent(m_pHuskAni);
        m_vFixPos = GetPos();

        AddComponent<Rigidbody>();
        Rigidbody* rigidbody = GetComponent<Rigidbody>();
        rigidbody->SetGround(true);
    }

    Husk::~Husk()
    {
    }

    void Husk::Tick()
    {
            if (nullptr != m_pPlayer)
            {
                m_vTargetPos = m_pPlayer->GetPos();            
                Vector2 Area = GetPos();
                //if (abs(Area.x - m_vTargetPos.x) < 800 && abs(Area.y - m_vTargetPos.y) < 400)
                if (abs(Area.x - m_vTargetPos.x) < 2000 && abs(Area.y - m_vTargetPos.y) < 2000)
                    m_bTar = true;
                else
                    m_bTar = false;
            }

            if(m_bTar)
            {
                if (m_bFix == false)
                {
                    m_vFixPos = GetPos();
                    m_bFix = true;
                }

                switch (m_tMonster.action)
                {
                case ACTION::End:
                {
                    m_vTargetPos = m_pPlayer->GetPos();
                    Vector2 Area = GetPos();
                    if (abs(Area.x - m_vTargetPos.x) < 800 && abs(Area.y - m_vTargetPos.y) < 400)
                        m_tMonster.action = ACTION::None;
                }
                break;

                case ACTION::None:
                {
                    if (m_tMonster.direction == DIRECTION::Left)
                        m_pHuskAni->Play(L"Husk_Left_Standing", true);
                    if (m_tMonster.direction == DIRECTION::Right)
                        m_pHuskAni->Play(L"Husk_Right_Standing", true);

                    m_tMonster.action = ACTION::Standing;
                }
                break;
                case ACTION::Standing:
                {
                    s_fTimer += DT;
                    if (s_fTimer >= 4.0f)
                    {
                        if (m_vFixPos.x == GetPos().x)
                        {
                            m_pHuskAni->Play(L"Husk_Left_Turn", false);
                            m_tMonster.direction = DIRECTION::Left;
                        }
                        if (m_vFixPos.x + 300 <= GetPos().x)
                        {
                            m_pHuskAni->Play(L"Husk_Left_Turn", false);
                            m_tMonster.direction = DIRECTION::Left;
                        }
                        if (m_vFixPos.x - 300 >= GetPos().x)
                        {
                            m_pHuskAni->Play(L"Husk_Right_Turn", false);
                            m_tMonster.direction = DIRECTION::Right;
                        }
                        m_tMonster.action = ACTION::Turn;
                        s_fTimer = 0.f;
                    }
                }
                break;
                case ACTION::Turn:
                {
                    s_fTimer += DT;
                    if (s_fTimer >= 0.2f)
                    {
                        if (m_tMonster.direction == DIRECTION::Left)
                            m_pHuskAni->Play(L"Husk_Left_Walk", true);
                        if (m_tMonster.direction == DIRECTION::Right)
                            m_pHuskAni->Play(L"Husk_Right_Walk", true);

                        m_tMonster.action = ACTION::Walk;
                        s_fTimer = 0.f;
                    }
                }
                break;
                case ACTION::Walk:
                {
                    if (m_tMonster.direction == DIRECTION::Left)
                    {
                       if(!m_bLeftWalkCheak)
                       {
                           m_vMovePos = GetPos();
                           m_vMovePos.x -= 200.f * DT;
                           SetPos(m_vMovePos);
                       }
                        if (m_vMovePos.x <= m_vFixPos.x - 320)
                        {
                            m_tMonster.action = ACTION::None;
                        }
                    }
                    if (m_tMonster.direction == DIRECTION::Right)
                    {
                        if(!m_bRightWalkCheak)
                        {
                            m_vMovePos = GetPos();
                            m_vMovePos.x += 200.f * DT;
                            SetPos(m_vMovePos);
                        }
                        if (m_vMovePos.x >= m_vFixPos.x + 320)
                        {
                            m_tMonster.action = ACTION::None;
                        }
                    }
                }
                break;
                case ACTION::Detect:
                {
                    m_vMovePos = GetPos();
                    Vector2 direction = m_vTargetPos - m_vMovePos;
                    direction.Normalize();
                    m_vMovePos.x += direction.x * 100 * DT;
                    SetPos(m_vMovePos);

                    if (!m_bTarget)
                    {
                        if (m_tMonster.direction == DIRECTION::Left)
                            m_pHuskAni->Play(L"Husk_Left_Walk", true);
                        if (m_tMonster.direction == DIRECTION::Right)
                            m_pHuskAni->Play(L"Husk_Right_Walk", true);
                        m_bTarget = true;
                    }
                    //탐지 범위를 벗어나면 None 상태로 변경
                    if (m_vMovePos.x - 401 > m_vTargetPos.x || m_vMovePos.y - 201 > m_vTargetPos.y
                        || m_vMovePos.x + 401 < m_vTargetPos.x || m_vMovePos.y + 100 < m_vTargetPos.y)
                    {
                        m_tMonster.action = ACTION::None;
                    }
                    //조건 확인 후 탐지 영역 내에서 오른쪽 왼쪽으로 추적 or 타켓팅으로 변경
                    Husk_Right_Turn();
                    Husk_Left_Turn();
                }
                break;
                case ACTION::Target:
                {
                    s_fTimer += DT;
                    if (s_fTimer >= 0.5f)
                    {
                        if (m_tMonster.direction == DIRECTION::Left)
                        {
                            m_pHuskAni->Play(L"Husk_Left_Attack", true);
                        }
                        if (m_tMonster.direction == DIRECTION::Right)
                        {
                            m_pHuskAni->Play(L"Husk_Right_Attack", true);
                        }
                        m_tMonster.action = ACTION::Attack;
                        m_vAttackPos = GetPos();
                        s_fTimer = 0.f;
                    }

                }
                break;
                case ACTION::Attack:
                {

                    if (m_tMonster.direction == DIRECTION::Left)
                    {
                        if (m_vAttackPos.x - 400 <= m_vMovePos.x && !m_bLeftWalkCheak)
                            m_vMovePos.x -= 400.f * DT;
                        else
                        {
                            m_pHuskAni->Play(L"Husk_Left_Attack_Cool", true);
                            m_tMonster.action = ACTION::Attack_Cool;
                        }
                    }
                    if (m_tMonster.direction == DIRECTION::Right)
                    {
                        if (m_vAttackPos.x + 400 >= m_vMovePos.x && !m_bRightWalkCheak)
                            m_vMovePos.x += 400 * DT;
                        else
                        {
                            m_pHuskAni->Play(L"Husk_Right_Attack_Cool", true);
                            m_tMonster.action = ACTION::Attack_Cool;
                        }
                    }
                    SetPos(m_vMovePos);

                }
                break;
                case ACTION::Attack_Cool:
                {
                    Vector2 Pos = GetPos();
                    s_fTimer += DT;
                    if (s_fTimer >= 1.0f)
                    {
                        if (m_vTargetPos.x > Pos.x)
                        {
                            m_tMonster.direction = DIRECTION::Right;
                            m_tMonster.action = ACTION::Detect;
                            m_bTarget = false;
                        }

                        if (m_vTargetPos.x < Pos.x)
                        {
                            m_tMonster.direction = DIRECTION::Left;
                            m_tMonster.action = ACTION::Detect;
                            m_bTarget = false;
                        }


                        s_fTimer = 0.f;
                    }
                }
                break;
                case ACTION::Damage:
                {                   
                    
                    if (m_Hp == 0)
                    {
                        if (m_tMonster.direction == DIRECTION::Left)
                        {
                            m_pHuskAni->Play(L"Husk_Right_Death_Air", false);
                        }
                        if (m_tMonster.direction == DIRECTION::Right)
                        {
                            m_pHuskAni->Play(L"Husk_Left_Death_Air", false);
                        }
                        m_vAttackPos = GetPos();
                        m_vMovePos = GetPos();
                        m_tMonster.action = ACTION::Death_Air;
                        s_fTimer;
                    }
                    //오른쪽 으로 밀려야 함..
                    if (m_tMonster.direction == DIRECTION::Left)
                    {    
                       if (m_vAttackPos.x + 100 >= m_vMovePos.x && !m_bRightWalkCheak)
                           m_vMovePos.x += 700.f * DT;
                       else                  
                       m_tMonster.action = ACTION::None;                         
                    }
                    //왼쪽 으로 밀려야 함..
                    if (m_tMonster.direction == DIRECTION::Right)
                    {
                        if (m_vAttackPos.x - 100 <= m_vMovePos.x && !m_bLeftWalkCheak)
                            m_vMovePos.x -= 700.f * DT;
                        else
                        m_tMonster.action = ACTION::None;
                    }
                    SetPos(m_vMovePos);

                }
                break;
                case ACTION::Death_Air:
                {
                    s_fTimer = 0.f;
                    CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, false);
                    CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, false);
                    if (m_tMonster.direction == DIRECTION::Left)
                    {
                        if (m_vAttackPos.x + 80 >= m_vMovePos.x && !m_bRightWalkCheak)
                            m_vMovePos.x += 400.f * DT;
                        else
                        {
                            m_pHuskAni->Play(L"Husk_Right_Death", false);
                            m_tMonster.action = ACTION::Death;
                            
                        }
                    }
                    //왼쪽 으로 밀려야 함..
                    if (m_tMonster.direction == DIRECTION::Right)
                    {
                        if (m_vAttackPos.x - 80 <= m_vMovePos.x && !m_bLeftWalkCheak)
                            m_vMovePos.x -= 400.f * DT;
                        else
                        {
                            m_pHuskAni->Play(L"Husk_Left_Death", false);
                            m_tMonster.action = ACTION::Death;                      
                        }
                    }
                    SetPos(m_vMovePos);

                }
                break;
                case ACTION::Death:
                {
                    CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, false);
                    CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, false);
                    s_fTimer += DT;
                    if (s_fTimer >= 2.1f)
                    {
                        
                        CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player, true);
                        CollisionManager::SetLayer(eColliderLayer::Monster, eColliderLayer::Player_Projecttile, true);                       
                        s_fTimer = 0.f;
                        Hornet_Key* key = ya::object::Instantiate<Hornet_Key>(eColliderLayer::GameObject);

                        this->Death();
                    }
                }
                break;
                }


                m_vMovePos = GetPos();
                if (m_vTargetPos.x >= m_vMovePos.x - 400 && m_vTargetPos.y >= m_vMovePos.y - 200
                    && m_vTargetPos.x <= m_vMovePos.x + 400 && m_vTargetPos.y <= m_vMovePos.y + 100)
                {

                    if (!(m_tMonster.action == ACTION::Detect) && !(m_tMonster.action == ACTION::Target)
                        && !(m_tMonster.action == ACTION::Attack) && !(m_tMonster.action == ACTION::Attack_Cool)
                        && !(m_tMonster.action == ACTION::Damage) && !(m_tMonster.action == ACTION::Death_Air)
                        && !(m_tMonster.action == ACTION::Death))
                    {

                        s_fTimer += DT;
                        if (m_vTargetPos.x < m_vMovePos.x)
                        {
                            m_tMonster.direction = DIRECTION::Left;
                            m_pHuskAni->Play(L"Husk_Left_Turn", false);
                            m_bTarget = false;
                        }
                        if (m_vTargetPos.x > m_vMovePos.x)
                        {
                            m_tMonster.direction = DIRECTION::Right;
                            m_pHuskAni->Play(L"Husk_Right_Turn", false);
                            m_bTarget = false;
                        }
                        if (s_fTimer >= 0.2f)
                        {
                            m_tMonster.action = ACTION::Detect;
                            s_fTimer = 0.f;
                        }

                    }
                }
            }
            else
            {
                m_tMonster.action = ACTION::None;
                m_pHuskAni->Play(L"Husk_Right_Standing", true);
            }
        

        GameObject::Tick();
    }

    void Husk::Render(HDC hdc)
    {
      

        GameObject::Render(hdc);
    }

    void Husk::OnCollisionEnter(Collider* other)
    {
        if (Player_Attack* playerObj = dynamic_cast<Player_Attack*>(other->GetOwner()))
        {
            Vector2 DamagePos = m_vTargetPos;
            Vector2 Pos = GetPos();
            //오른쪽으로 밀려야 함..
            if (Pos.x > DamagePos.x)
                m_tMonster.direction = DIRECTION::Left;
            //왼쪽으로 미밀려야 함..
            if (Pos.x < DamagePos.x)
                m_tMonster.direction = DIRECTION::Right;

            m_vAttackPos = GetPos();
            m_vMovePos = GetPos();

            m_tMonster.action = ACTION::Damage; 

            if (playerObj->GetSkill() == Player_Skill::Magic || playerObj->GetSkill() == Player_Skill::Magic_Walk)
            {
                playerObj->SetHit(true);
                if (m_Hp > 2)
                {
                    m_Hp -= 3;
                }
                else if (m_Hp < 3)
                {
                    m_Hp = 0;
                }
                m_pSound->Play(false);
            }
            else
            {
                --m_Hp;
                m_pSound->Play(false);
            }

            if (nullptr != m_pPlayer)
            {
                Player* play = (Player*)m_pPlayer;
                if (play->GetMp() >= 6)
                    return;
                int mp = play->GetMp();
                ++mp;
                play->SetMp(mp);
            }
        }






    }

    void Husk::OnCollisionStay(Collider* other)
    {


    }

    void Husk::OnCollisionExit(Collider* other)
    {


    }


    void Husk::AnimationInitialize()
    {
        //기본 서있는 모션
        m_pHuskAni->CreateAnimation(L"Husk_Left_Standing", m_pHuskImg
            , Vector2(0.0f, 0.0f), Vector2(106.0f, 129.0f)
            , Vector2(0.0f, 0.0f), 6, 0.3f);
        m_pHuskAni->CreateAnimation(L"Husk_Right_Standing", m_pHuskImg
            , Vector2(0.0f, 129.0f), Vector2(106.0f, 129.0f)
            , Vector2(0.0f, 0.0f), 6, 0.3f);
        //좌우 바뀌는 모션 
        m_pHuskAni->CreateAnimation(L"Husk_Left_Turn", m_pHuskImg
            , Vector2(636.0f, 0.0f), Vector2(104.0f, 129.0f)
            , Vector2(0.0f, 0.0f), 2, 0.1f);
        m_pHuskAni->CreateAnimation(L"Husk_Right_Turn", m_pHuskImg
            , Vector2(636.0f, 129.0f), Vector2(104.0f, 129.0f)
            , Vector2(0.0f, 0.0f), 2, 0.1f);
        //걷는 모션
        m_pHuskAni->CreateAnimation(L"Husk_Left_Walk", m_pHuskImg
            , Vector2(0.0f, 258.0f), Vector2(115.0f, 129.0f)
            , Vector2(0.0f, 0.0f), 7, 0.1f);
        m_pHuskAni->CreateAnimation(L"Husk_Right_Walk", m_pHuskImg
            , Vector2(0.0f, 387.0f), Vector2(115.0f, 129.0f)
            , Vector2(0.0f, 0.0f), 7, 0.1f);
        //공격 타겟팅 모션
        m_pHuskAni->CreateAnimation(L"Husk_Left_Target", m_pHuskImg
            , Vector2(0.0f, 516.0f), Vector2(114.0f, 138.0f)
            , Vector2(0.0f, 0.0f), 5, 0.1f);
        m_pHuskAni->CreateAnimation(L"Husk_Right_Target", m_pHuskImg
            , Vector2(0.0f, 654.0f), Vector2(114.0f, 138.0f)
            , Vector2(0.0f, 0.0f), 5, 0.1f);
        //죽기 직전 날라갈때 모션
        m_pHuskAni->CreateAnimation(L"Husk_Left_Death_Air", m_pHuskImg
            , Vector2(570.0f, 516.0f), Vector2(128.0f, 101.0f)
            , Vector2(0.0f, 0.0f), 1, 0.1f);
        m_pHuskAni->CreateAnimation(L"Husk_Right_Death_Air", m_pHuskImg
            , Vector2(698.0f, 516.0f), Vector2(128.0f, 101.0f)
            , Vector2(0.0f, 0.0f), 1, 0.1f);
        //공격 모션(달려들기)
        m_pHuskAni->CreateAnimation(L"Husk_Left_Attack", m_pHuskImg
            , Vector2(0.0f, 792.0f), Vector2(122.0f, 123.0f)
            , Vector2(0.0f, 0.0f), 4, 0.1f);
        m_pHuskAni->CreateAnimation(L"Husk_Right_Attack", m_pHuskImg
            , Vector2(0.0f, 915.0f), Vector2(122.0f, 123.0f)
            , Vector2(0.0f, 0.0f), 4, 0.1f);
        //공격 후 쿨타임
        m_pHuskAni->CreateAnimation(L"Husk_Left_Attack_Cool", m_pHuskImg
            , Vector2(488.0f, 792.0f), Vector2(106.0f, 123.0f)
            , Vector2(0.0f, 0.0f), 1, 0.1f);
        m_pHuskAni->CreateAnimation(L"Husk_Right_Attack_Cool", m_pHuskImg
            , Vector2(488.0f, 915.0f), Vector2(106.0f, 123.0f)
            , Vector2(0.0f, 0.0f), 1, 0.1f);


        //죽는 모션
        m_pHuskAni->CreateAnimation(L"Husk_Left_Death", m_pHuskImg
            , Vector2(0.0f, 1038.0f), Vector2(136.0f, 116.0f)
            , Vector2(0.0f, 0.0f), 8, 0.1f);
        m_pHuskAni->CreateAnimation(L"Husk_Right_Death", m_pHuskImg
            , Vector2(0.0f, 1154.0f), Vector2(136.0f, 116.0f)
            , Vector2(0.0f, 0.0f), 8, 0.1f);

    }

    void Husk::Husk_Left_Turn()
    {
        //m_vMovePos = GetPos();

        if (m_tMonster.direction == DIRECTION::Right)
        {
            if (m_vMovePos.x + 300 >= m_vTargetPos.x)
            {
                //공격 범위 안으로 진입하면 타겟팅 상태로 변경
                m_pHuskAni->Play(L"Husk_Right_Target", false);
                m_tMonster.action = ACTION::Target;
            }

            if (m_vMovePos.x - 100 > m_vTargetPos.x)
            {
                //플레이어가 반대편으로 돌아가면 반대로 이동
                m_pHuskAni->Play(L"Husk_Left_Turn", false);
                s_fTimer += DT;
                if (s_fTimer > 0.2f)
                {
                    m_tMonster.direction = DIRECTION::Left;
                    m_tMonster.action = ACTION::Turn;
                    m_bTarget = false;
                    s_fTimer = 0.f;
                }
            }
        }


    }

    void Husk::Husk_Right_Turn()
    {
        //m_vMovePos = GetPos();
        if (m_tMonster.direction == DIRECTION::Left)
        {
            if (m_vMovePos.x - 300 <= m_vTargetPos.x)
            {
                //공격 범위 안으로 진입하면 타겟팅 상태로 변경
                m_pHuskAni->Play(L"Husk_Left_Target", false);
                m_tMonster.action = ACTION::Target;
            }

            if (m_vMovePos.x + 100 < m_vTargetPos.x)
            {
                //플레이어가 반대편으로 돌아가면 반대로 이동
                m_pHuskAni->Play(L"Husk_Right_Turn", false);
                s_fTimer += DT;
                if (s_fTimer > 0.2f)
                {
                    m_tMonster.direction = DIRECTION::Right;
                    m_tMonster.action = ACTION::Turn;
                    m_bTarget = false;
                    s_fTimer = 0.f;
                }
            }
        }
    }

}