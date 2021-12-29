#include "stdafx.h"
#include "Vector4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

const Vector4 Vector4::Zero = Vector4(0.0f);
const Vector4 Vector4::Red(1.0f, 0.0f, 0.0f, 1.0f);
const Vector4 Vector4::Blue(0.0f, 0.0f, 1.0f, 1.0f);
const Vector4 Vector4::Green(0.0f, 1.0f, 0.0f, 1.0f);
const Vector4 Vector4::White(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4 Vector4::Black(0.0f, 0.0f, 0.0f, 1.0f);

Vector4::Vector4()
    : x(0.0f)
    , y(0.0f)
    , z(0.0f)
    , w(0.0f)
{
}

Vector4::Vector4(const float & x, const float & y, const float & z, const float & w)
    : x(x)
    , y(y)
    , z(z)
    , w(w)
{
}

Vector4::Vector4(const float & value)
    : x(value)
    , y(value)
    , z(value)
    , w(value)
{
}

//0xFF555566  ->  24  =  0x000000FF
//0xFF555566  ->  16  =  0x0000FF55  -> & 0x000000FF -> 0x00000055
Vector4::Vector4(const UINT& value)
{
    const float f = 1.0f / 255.0f;

    x = f * static_cast<float>(static_cast<unsigned char>(value >> 16));
    y = f * static_cast<float>(static_cast<unsigned char>(value >> 8));
    z = f * static_cast<float>(static_cast<unsigned char>(value >> 0));
    w = f * static_cast<float>(static_cast<unsigned char>(value >> 24));
}

Vector4::Vector4(const Vector4 & rhs)
    : x(rhs.x)
    , y(rhs.y)
    , z(rhs.z)
    , w(rhs.w)
{
}

Vector4::Vector4(const Vector2 & rhs)
    : x(rhs.x)
    , y(rhs.y)
    , z(0.0f)
    , w(0.0f)
{
}

Vector4::Vector4(const Vector3 & rhs)
    : x(rhs.x)
    , y(rhs.y)
    , z(rhs.z)
    , w(0.0f)
{
}

Vector4::Vector4(const Quaternion& rhs)
    : x(rhs.x)
    , y(rhs.y)
    , z(rhs.z)
    , w(rhs.w)
{
}

Vector4::operator UINT() const
{
    UINT temp_r = x >= 1.0f ? 0xff : x <= 0 ? 0x00 : static_cast<UINT>(x * 255.0f); //0x00660000
    UINT temp_g = y >= 1.0f ? 0xff : y <= 0 ? 0x00 : static_cast<UINT>(y * 255.0f); //0x00005500
    UINT temp_b = z >= 1.0f ? 0xff : z <= 0 ? 0x00 : static_cast<UINT>(z * 255.0f); //0x00000055
    UINT temp_a = w >= 1.0f ? 0xff : w <= 0 ? 0x00 : static_cast<UINT>(w * 255.0f); //0xFF000000

    return (temp_a << 24) | (temp_r << 16) | (temp_g << 8) | (temp_b << 0);
}

auto Vector4::Normalize() const -> const Vector4
{
    auto factor = Length();
    factor = 1.0f / factor;

    return *this * factor;
}

void Vector4::Normalize()
{
    auto factor = Length();
    factor = 1.0f / factor;

    *this *= factor;
}
