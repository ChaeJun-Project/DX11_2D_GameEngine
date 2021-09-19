#pragma once

class Vector4 final
{
public:
    Vector4();
    Vector4(const float& x, const float& y, const float& z, const float& w);
    Vector4(const float& value);
    Vector4(const Vector4& rhs);
    Vector4(const class Vector2& rhs);
    Vector4(const class Vector3& rhs);
    ~Vector4() = default;

    auto Length() const -> const float { return sqrtf(x * x + y * y + z * z + w * w); }
    auto LengthSq() const -> const float { return x * x + y * y + z * z + w * w; }
    auto Normalize() const -> const Vector4;
    void Normalize();

    operator float*() { return &x; }
    operator const float*() const { return &x; }

    Vector4& operator=(const Vector4& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; return *this; }

    const Vector4 operator+(const Vector4& rhs) const { return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
    const Vector4 operator-(const Vector4& rhs) const { return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
    const Vector4 operator*(const Vector4& rhs) const { return Vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w); }
    const Vector4 operator/(const Vector4& rhs) const { return Vector4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w); }

    const Vector4 operator+(const float& rhs) const { return Vector4(x + rhs, y + rhs, z + rhs, w + rhs); }
    const Vector4 operator-(const float& rhs) const { return Vector4(x - rhs, y - rhs, z - rhs, w - rhs); }
    const Vector4 operator*(const float& rhs) const { return Vector4(x * rhs, y * rhs, z * rhs, w * rhs); }
    const Vector4 operator/(const float& rhs) const { return Vector4(x / rhs, y / rhs, z / rhs, w / rhs); }

    void operator+=(const Vector4& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; }
    void operator-=(const Vector4& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; }
    void operator*=(const Vector4& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w; }
    void operator/=(const Vector4& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; w /= rhs.w; }

    void operator+=(const float& rhs) { x += rhs; y += rhs; z += rhs; w += rhs; }
    void operator-=(const float& rhs) { x -= rhs; y -= rhs; z -= rhs; w -= rhs; }
    void operator*=(const float& rhs) { x *= rhs; y *= rhs; z *= rhs; w *= rhs; }
    void operator/=(const float& rhs) { x /= rhs; y /= rhs; z /= rhs; w /= rhs; }

    const bool operator==(const Vector4& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
    const bool operator!=(const Vector4& rhs) const { return !(*this == rhs); }

public:
    float x;
    float y;
    float z;
    float w;
};