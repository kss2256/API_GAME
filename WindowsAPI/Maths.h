#pragma once

#define PI 3.14159265f

#include <cmath>
#include <algorithm>

namespace ya
{
    struct Vector2
    {
        static Vector2 Zero;
        static Vector2 One;
        static Vector2 Up;
        static Vector2 Down;
        static Vector2 Right;
        static Vector2 Left;

        float x;
        float y;

        Vector2() = default;



        Vector2(const Vector2&) = default;
        Vector2& operator=(const Vector2&) = default;

        Vector2(Vector2&&) = default;
        Vector2& operator=(Vector2&&) = default;

        Vector2 operator -()
        {
            return Vector2(-x, -y);
        }

        Vector2 operator+(const Vector2& other)
        {
            Vector2 temp;
            temp.x = x + other.x;
            temp.y = y + other.y;

            return temp;
        }

        Vector2 operator-(const Vector2& other)
        {
            Vector2 temp;
            temp.x = x - other.x;
            temp.y = y - other.y;

            return temp;
        }

        Vector2 operator*(const Vector2& other)
        {
            Vector2 temp;
            temp.x = x * other.x;
            temp.y = y * other.y;

            return temp;
        }

        Vector2 operator*(const float& value)
        {
            Vector2 temp;
            temp.x = x * value;
            temp.y = y * value;

            return temp;
        }
        
        Vector2 operator/(const float& value)
        {
            Vector2 temp;

            temp.x = x / value;
            temp.y = y / value;

            return temp;
        }

        void operator/=(const float& value)
        { 
                x = x / value;
                y = y / value;           
        }
        void operator/=(const int& i)
        {
            x = (int)(x / i);
            y = (int)(y / i);
        }

        Vector2& operator +=(const Vector2& other)
        {
            x += other.x;
            y += other.y;

            return *this;
        }

        Vector2& operator *=(const float value)
        {
            x *= value;
            y *= value;

            return *this;
        }

        Vector2& operator -=(const Vector2& other)
        {
            x -= other.x;
            y -= other.y;

            return *this;
        }

        bool operator ==(const Vector2& other)
        {
            return (x == other.x && y == other.y);
        }
        //bool operator !=(const Vector2& other)
        //{
        //    return (x != other.x && y != other.y);
        //}

        constexpr Vector2(float _x, float _y) noexcept
            : x(_x)
            , y(_y)
        {

        }
        explicit Vector2(_In_reads_(2) const float* pArray)  noexcept
            : x(pArray[0])
            , y(pArray[1])
        {

        }

        void clear()
        {
            x = 0.0f;
            y = 0.0f;
        }

        float Length()
        {
            return sqrtf(x * x + y * y);
        }

        Vector2& Normalize()
        {
            float length = Length();
            x /= length;
            y /= length;

            return *this;
        }
    };
    typedef Vector2 Pos;
    typedef Vector2 Size;
}


namespace ya::math
{
    inline Vector2 Rotate(Vector2 vector, float degree)
    {
        float radian = (degree / 180.f) * PI;
        vector.Normalize();
        float x = cosf(radian) * vector.x - sinf(radian) * vector.y;
        float y = sinf(radian) * vector.x + cosf(radian) * vector.y;

        return Vector2(x, y);
    }

    inline float Dot(Vector2& v1, Vector2& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    inline float Cross(Vector2 v1, Vector2 v2)
    {
        return v1.x * v2.y - v1.y * v2.x;
    }

    inline float DegreeToRadian(float degree)
    {
        return degree * PI / 180.0f;
    }

    inline float RadianToDegree(float radian)
    {
        return radian * (180.0f / PI);
    }
}

namespace ya
{
    struct Sprite
    {
        Vector2 leftTop; // 좌측상단 시작좌표
        Vector2 size; // 좌측상단으로부터 잘라낼 가로 세로 길이
        Vector2 offset;
        float duration; //해당 프레임 유지 시간


        void operator = (Sprite _other)
        {
            leftTop = _other.leftTop;
            size = _other.size;
            offset = _other.offset;
            duration = _other.duration;
        }
        void operator = (std::vector<Sprite> _other)
        {
            for (size_t i = 0; i < _other.size(); ++i)
            {
                leftTop = _other[i].leftTop;
                size = _other[i].size;
                offset = _other[i].offset;
                duration = _other[i].duration;
            }
        }

        Sprite()
            : leftTop(0.0f, 0.0f)
            , size(0.0f, 0.0f)
            , offset(0.0f, 0.0f)
            , duration(0.0f)
        {

        }




    };


}


namespace ya
{
    struct State
    {
        Attack          attack;
        SITUATION       situation;//현재 상태 방향 체크
        DIRECTION       direction;//현재 바라보고 있는 방향
        Vector2         Pos;     //위치정보
        PLAYER_BGM      bgm;
        float 	        Time_Limit; //다른 키조작 영향 안받는 시간
        bool            Limit_Cheak;//제한 조건 체크

    };
}

namespace ya
{

    struct Command
    {
        DIRECTION   direction;    //방향
        ACTION      action;       //동작
        
    };


}