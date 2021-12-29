#pragma once

class Vector3 final
{
public:
    static const Vector3 Zero;
    static const Vector3 One;
    static const Vector3 Right;
    static const Vector3 Left;
    static const Vector3 Up;
    static const Vector3 Down;
    static const Vector3 Forward;
    static const Vector3 Back;
    static const Vector3 Infinity;
    static const Vector3 NegInfinity;

public:
    static auto Distance(const Vector3& lhs, const Vector3& rhs) -> const float;
    static auto DistanceSq(const Vector3& lhs, const Vector3& rhs) -> const float;
    static auto Dot(const Vector3& lhs, const Vector3& rhs) -> const float;
    static auto Cross(const Vector3& lhs, const Vector3& rhs) -> const Vector3;
    static auto Normalize(const Vector3& rhs) -> const Vector3;

    static auto TransformCoord(const Vector3& lhs, const class Matrix& rhs) -> const Vector3;
    static auto TransformNormal(const Vector3& lhs, const class Matrix& rhs) -> const Vector3;

	static auto Lerp(const Vector3& lhs, const Vector3& rhs, const float& factor) -> const Vector3;

public:
    Vector3();
    Vector3(const float& x, const float& y, const float& z);
    Vector3(const float& value);
    Vector3(const Vector3& rhs);
    Vector3(const class Vector2& rhs);
	Vector3(const class Vector4& rhs);
    ~Vector3() = default;

    auto Length() const -> const float { return sqrtf(x * x + y * y + z * z); }
    auto LengthSq() const -> const float { return x * x + y * y + z * z; }
    auto Volume() const -> const float { return x * y * z; }
    auto Dot(const Vector3& rhs) const -> const float { return Dot(*this, rhs); }
    auto Cross(const Vector3& rhs) const -> const Vector3 { return Cross(*this, rhs); }
    auto Absolute() const -> const Vector3 { return Vector3(fabs(x), fabs(y), fabs(z)); }
    auto Normalize() const -> const Vector3;
    void Normalize();
    void Floor();

    operator float*() { return &x; }
    operator const float*() const { return &x; }

    Vector3& operator=(const Vector3& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; return *this; }

    const Vector3 operator+(const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
    const Vector3 operator-(const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
    const Vector3 operator*(const Vector3& rhs) const { return Vector3(x * rhs.x, y * rhs.y, z * rhs.z); }
    const Vector3 operator/(const Vector3& rhs) const { return Vector3(x / rhs.x, y / rhs.y, z / rhs.z); }

    const Vector3 operator+(const float& rhs) const { return Vector3(x + rhs, y + rhs, z + rhs); }
    const Vector3 operator-(const float& rhs) const { return Vector3(x - rhs, y - rhs, z - rhs); }
    const Vector3 operator*(const float& rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
    const Vector3 operator/(const float& rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }

    void operator+=(const Vector3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; }
    void operator-=(const Vector3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; }
    void operator*=(const Vector3& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; }
    void operator/=(const Vector3& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; }

    void operator+=(const float& rhs) { x += rhs; y += rhs; z += rhs; }
    void operator-=(const float& rhs) { x -= rhs; y -= rhs; z -= rhs; }
    void operator*=(const float& rhs) { x *= rhs; y *= rhs; z *= rhs; }
    void operator/=(const float& rhs) { x /= rhs; y /= rhs; z /= rhs; }

    const bool operator==(const Vector3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    const bool operator!=(const Vector3& rhs) const { return !(*this == rhs); }

public:
    float x;
    float y;
    float z;
};

inline Vector3 operator*(const float& lhs, const Vector3& rhs) { return rhs * lhs; }