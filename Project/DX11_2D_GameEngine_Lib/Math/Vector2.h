#pragma once

class Vector2 final
{
public:
    static const Vector2 Zero;
    static const Vector2 One;

public:
    Vector2();
    Vector2(const float& x, const float& y);
    Vector2(const float& value);
    Vector2(const Vector2& rhs);
    ~Vector2() = default;

    auto Length() const -> const float { return sqrtf(x * x + y * y); }
    auto LengthSq() const -> const float { return x * x + y * y; }
    auto Normalize() const -> const Vector2;
    void Normalize();

    operator float*() { return &x; }
    operator const float*() const { return &x; }

    Vector2& operator=(const Vector2& rhs) { x = rhs.x; y = rhs.y; return *this; }

    const Vector2 operator+(const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
    const Vector2 operator-(const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
    const Vector2 operator*(const Vector2& rhs) const { return Vector2(x * rhs.x, y * rhs.y); }
    const Vector2 operator/(const Vector2& rhs) const { return Vector2(x / rhs.x, y / rhs.y); }

    const Vector2 operator+(const float& rhs) const { return Vector2(x + rhs, y + rhs); }
    const Vector2 operator-(const float& rhs) const { return Vector2(x - rhs, y - rhs); }
    const Vector2 operator*(const float& rhs) const { return Vector2(x * rhs, y * rhs); }
    const Vector2 operator/(const float& rhs) const { return Vector2(x / rhs, y / rhs); }

    void operator+=(const Vector2& rhs) { x += rhs.x; y += rhs.y; }
    void operator-=(const Vector2& rhs) { x -= rhs.x; y -= rhs.y; }
    void operator*=(const Vector2& rhs) { x *= rhs.x; y *= rhs.y; }
    void operator/=(const Vector2& rhs) { x /= rhs.x; y /= rhs.y; }

    void operator+=(const float& rhs) { x += rhs; y += rhs; }
    void operator-=(const float& rhs) { x -= rhs; y -= rhs; }
    void operator*=(const float& rhs) { x *= rhs; y *= rhs; }
    void operator/=(const float& rhs) { x /= rhs; y /= rhs; }

    const bool operator==(const Vector2& rhs) const { return x == rhs.x && y == rhs.y; }
    const bool operator!=(const Vector2& rhs) const { return x != rhs.x || y != rhs.y; }

public:
    float x;
    float y;
};