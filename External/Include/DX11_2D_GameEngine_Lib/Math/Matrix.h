#pragma once
#include "Vector3.h"
#include "Quaternion.h"

class Matrix final
{
public:
    static const Matrix Identity;

public:
    static const Matrix Scaling(const float& x, const float& y, const float& z);
    static const Matrix Scaling(const Vector3& scale);
    static const Matrix Scaling(const float& scale);

    static const Matrix RotationQuaternion(const Quaternion& rotation);
    static const Quaternion RotationMatrixToQuaternion(const Matrix& rotation);

    static const Matrix Translation(const float& x, const float& y, const float& z);
    static const Matrix Translation(const Vector3& translation);

    static const Matrix LookAtLH(const class Vector3& eye, const class Vector3& at, const class Vector3& up);
    static const Matrix OrthoLH(const float& w, const float& h, const float& zn, const float& zf);
    static const Matrix OrthoOffCenterLH(const float& l, const float& r, const float& b, const float& t, const float& zn, const float& zf);
    static const Matrix PerspectiveFovLH(const float& fov, const float& aspect, const float& zn, const float& zf);

    static const Matrix Inverse(const Matrix& rhs);
    static const Matrix Transpose(const Matrix& rhs);

public:
    Matrix();
    Matrix
    (
        const float& _11, const float& _12, const float& _13, const float& _14,
        const float& _21, const float& _22, const float& _23, const float& _24,
        const float& _31, const float& _32, const float& _33, const float& _34,
        const float& _41, const float& _42, const float& _43, const float& _44
    );
    Matrix(const Vector3& scale, const Quaternion& rotation, const Vector3& translation);
    ~Matrix() = default;

    void SetIdentity();

    const Vector3 GetScale();
    const Quaternion GetRotation();
    const Vector3 GetTranslation() { return Vector3(_41, _42, _43); }

    const Matrix Inverse() const { return Inverse(*this); }
    const Matrix Transpose() const { return Transpose(*this); }
    void Transpose() { *this = Transpose(*this); }

    void Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation);

    operator float*() { return &_11; }
    operator const float*() const { return &_11; }

	Vector3 operator*(const Vector3& rhs) const;
    const Matrix operator*(const Matrix& rhs) const;
    const bool operator==(const Matrix& rhs) const;
    const bool operator!=(const Matrix& rhs) const { return !(*this == rhs); }

public:
    float _11, _21, _31, _41;
    float _12, _22, _32, _42;
    float _13, _23, _33, _43;
    float _14, _24, _34, _44;
};

inline Vector3 operator*(const Vector3& lhs, const Matrix& rhs) { return rhs*lhs; }