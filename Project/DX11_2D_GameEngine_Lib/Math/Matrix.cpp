#include "stdafx.h"
#include "Matrix.h"

const Matrix Matrix::Identity
(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
);

const Matrix Matrix::Scaling(const float & x, const float & y, const float & z)
{
    return Matrix
    (
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    );
}

const Matrix Matrix::Scaling(const Vector3 & scale)
{
    return Scaling(scale.x, scale.y, scale.z);
}

const Matrix Matrix::Scaling(const float & scale)
{
    return Scaling(scale, scale, scale);
}

const Matrix Matrix::RotationQuaternion(const Quaternion & rotation)
{
    float num9 = rotation.x * rotation.x;
    float num8 = rotation.y * rotation.y;
    float num7 = rotation.z * rotation.z;
    float num6 = rotation.x * rotation.y;
    float num5 = rotation.z * rotation.w;
    float num4 = rotation.z * rotation.x;
    float num3 = rotation.y * rotation.w;
    float num2 = rotation.y * rotation.z;
    float num = rotation.x * rotation.w;

    return Matrix
    (
        1.0f - (2.0f * (num8 + num7)),
        2.0f * (num6 + num5),
        2.0f * (num4 - num3),
        0.0f,
        2.0f * (num6 - num5),
        1.0f - (2.0f * (num7 + num9)),
        2.0f * (num2 + num),
        0.0f,
        2.0f * (num4 + num3),
        2.0f * (num2 - num),
        1.0f - (2.0f * (num8 + num9)),
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f
    );
}

const Quaternion Matrix::RotationMatrixToQuaternion(const Matrix & rotation)
{
    Quaternion quaternion;

    float sq;
    float half;
    float scale = rotation._11 + rotation._22 + rotation._33;

    if (scale > 0.0f)
    {
        sq = sqrt(scale + 1.0f);
        quaternion.w = sq * 0.5f;
        sq = 0.5f / sq;

        quaternion.x = (rotation._23 - rotation._32) * sq;
        quaternion.y = (rotation._31 - rotation._13) * sq;
        quaternion.z = (rotation._12 - rotation._21) * sq;

        return quaternion;
    }
    if ((rotation._11 >= rotation._22) && (rotation._11 >= rotation._33))
    {
        sq = sqrt(1.0f + rotation._11 - rotation._22 - rotation._33);
        half = 0.5f / sq;

        quaternion.x = 0.5f * sq;
        quaternion.y = (rotation._12 + rotation._21) * half;
        quaternion.z = (rotation._13 + rotation._31) * half;
        quaternion.w = (rotation._23 - rotation._32) * half;

        return quaternion;
    }
    if (rotation._22 > rotation._33)
    {
        sq = sqrt(1.0f + rotation._22 - rotation._11 - rotation._33);
        half = 0.5f / sq;

        quaternion.x = (rotation._21 + rotation._12) * half;
        quaternion.y = 0.5f * sq;
        quaternion.z = (rotation._32 + rotation._23) * half;
        quaternion.w = (rotation._31 - rotation._13) * half;

        return quaternion;
    }
    sq = sqrt(1.0f + rotation._33 - rotation._11 - rotation._22);
    half = 0.5f / sq;

    quaternion.x = (rotation._31 + rotation._13) * half;
    quaternion.y = (rotation._32 + rotation._23) * half;
    quaternion.z = 0.5f * sq;
    quaternion.w = (rotation._12 - rotation._21) * half;

    return quaternion;
}

const Matrix Matrix::Translation(const float & x, const float & y, const float & z)
{
    return Matrix
    (
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    );
}

const Matrix Matrix::Translation(const Vector3 & translation)
{
    return Translation(translation.x, translation.y, translation.z);
}

const Matrix Matrix::LookAtLH(const Vector3 & eye, const Vector3 & at, const Vector3 & up)
{
    auto z_axis = Vector3::Normalize(at);
    auto x_axis = Vector3::Normalize(Vector3::Cross(up, z_axis));
    auto y_axis = Vector3::Normalize(Vector3::Cross(z_axis, x_axis));

    return Matrix
    (
        x_axis.x, y_axis.x, z_axis.x, 0,
        x_axis.y, y_axis.y, z_axis.y, 0,
        x_axis.z, y_axis.z, z_axis.z, 0,
        x_axis.Dot(eye), y_axis.Dot(eye), z_axis.Dot(eye), 1
    );
}

const Matrix Matrix::OrthoLH(const float & w, const float & h, const float & zn, const float & zf)
{
    return Matrix
    (
        2 / w, 0, 0, 0,
        0, 2 / h, 0, 0,
        0, 0, 1 / (zf - zn), 0,
        0, 0, zn / (zn - zf), 1
    );
}

const Matrix Matrix::OrthoOffCenterLH(const float & l, const float & r, const float & b, const float & t, const float & zn, const float & zf)
{
    return Matrix
    (
        2 / (r - l), 0, 0, 0,
        0, 2 / (t - b), 0, 0,
        0, 0, 1 / (zf - zn), 0,
        -(r + l) / 2, -(t + b) / 2, zn / (zn - zf), 1
    );
}

const Matrix Matrix::PerspectiveFovLH(const float & fov, const float & aspect, const float & zn, const float & zf)
{
    auto sy = cosf(fov / 2) / sinf(fov / 2);
    auto sx = sy / aspect;

    return Matrix
    (
        sx, 0, 0, 0,
        0, sy, 0, 0,
        0, 0, zf / (zf - zn), 1,
        0, 0, -zn * zf / (zf - zn), 0
    );
}

const Matrix Matrix::Inverse(const Matrix & rhs)
{
    float v0 = rhs._31 * rhs._42 - rhs._32 * rhs._41;
    float v1 = rhs._31 * rhs._43 - rhs._33 * rhs._41;
    float v2 = rhs._31 * rhs._44 - rhs._34 * rhs._41;
    float v3 = rhs._32 * rhs._43 - rhs._33 * rhs._42;
    float v4 = rhs._32 * rhs._44 - rhs._34 * rhs._42;
    float v5 = rhs._33 * rhs._44 - rhs._34 * rhs._43;

    float i11 = (v5 * rhs._22 - v4 * rhs._23 + v3 * rhs._24);
    float i21 = -(v5 * rhs._21 - v2 * rhs._23 + v1 * rhs._24);
    float i31 = (v4 * rhs._21 - v2 * rhs._22 + v0 * rhs._24);
    float i41 = -(v3 * rhs._21 - v1 * rhs._22 + v0 * rhs._23);

    float invDet = 1.0f / (i11 * rhs._11 + i21 * rhs._12 + i31 * rhs._13 + i41 * rhs._14);

    i11 *= invDet;
    i21 *= invDet;
    i31 *= invDet;
    i41 *= invDet;

    float i12 = -(v5 * rhs._12 - v4 * rhs._13 + v3 * rhs._14) * invDet;
    float i22 = (v5 * rhs._11 - v2 * rhs._13 + v1 * rhs._14) * invDet;
    float i32 = -(v4 * rhs._11 - v2 * rhs._12 + v0 * rhs._14) * invDet;
    float i42 = (v3 * rhs._11 - v1 * rhs._12 + v0 * rhs._13) * invDet;

    v0 = rhs._21 * rhs._42 - rhs._22 * rhs._41;
    v1 = rhs._21 * rhs._43 - rhs._23 * rhs._41;
    v2 = rhs._21 * rhs._44 - rhs._24 * rhs._41;
    v3 = rhs._22 * rhs._43 - rhs._23 * rhs._42;
    v4 = rhs._22 * rhs._44 - rhs._24 * rhs._42;
    v5 = rhs._23 * rhs._44 - rhs._24 * rhs._43;

    float i13 = (v5 * rhs._12 - v4 * rhs._13 + v3 * rhs._14) * invDet;
    float i23 = -(v5 * rhs._11 - v2 * rhs._13 + v1 * rhs._14) * invDet;
    float i33 = (v4 * rhs._11 - v2 * rhs._12 + v0 * rhs._14) * invDet;
    float i43 = -(v3 * rhs._11 - v1 * rhs._12 + v0 * rhs._13) * invDet;

    v0 = rhs._32 * rhs._21 - rhs._31 * rhs._22;
    v1 = rhs._33 * rhs._21 - rhs._31 * rhs._23;
    v2 = rhs._34 * rhs._21 - rhs._31 * rhs._24;
    v3 = rhs._33 * rhs._22 - rhs._32 * rhs._23;
    v4 = rhs._34 * rhs._22 - rhs._32 * rhs._24;
    v5 = rhs._34 * rhs._23 - rhs._33 * rhs._24;

    float i14 = -(v5 * rhs._12 - v4 * rhs._13 + v3 * rhs._14) * invDet;
    float i24 = (v5 * rhs._11 - v2 * rhs._13 + v1 * rhs._14) * invDet;
    float i34 = -(v4 * rhs._11 - v2 * rhs._12 + v0 * rhs._14) * invDet;
    float i44 = (v3 * rhs._11 - v1 * rhs._12 + v0 * rhs._13) * invDet;

    return Matrix
    (
        i11, i12, i13, i14,
        i21, i22, i23, i24,
        i31, i32, i33, i34,
        i41, i42, i43, i44
    );
}

const Matrix Matrix::Transpose(const Matrix & rhs)
{
    return Matrix
    (
        rhs._11, rhs._21, rhs._31, rhs._41,
        rhs._12, rhs._22, rhs._32, rhs._42,
        rhs._13, rhs._23, rhs._33, rhs._43,
        rhs._14, rhs._24, rhs._34, rhs._44
    );
}

Matrix::Matrix()
{
    SetIdentity();
}

Matrix::Matrix(const float & _11, const float & _12, const float & _13, const float & _14, const float & _21, const float & _22, const float & _23, const float & _24, const float & _31, const float & _32, const float & _33, const float & _34, const float & _41, const float & _42, const float & _43, const float & _44)
    : _11(_11), _12(_12), _13(_13), _14(_14)
    , _21(_21), _22(_22), _23(_23), _24(_24)
    , _31(_31), _32(_32), _33(_33), _34(_34)
    , _41(_41), _42(_42), _43(_43), _44(_44)
{
}

Matrix::Matrix(const Vector3 & scale, const Quaternion & rotation, const Vector3 & translation)
{
    Matrix R = RotationQuaternion(rotation);

    _11 = scale.x * R._11;  _12 = scale.x * R._12;  _13 = scale.x * R._13;  _14 = 0;
    _21 = scale.y * R._21;  _22 = scale.y * R._22;  _23 = scale.y * R._23;  _24 = 0;
    _31 = scale.z * R._31;  _32 = scale.z * R._32;  _33 = scale.z * R._33;  _34 = 0;
    _41 = translation.x;    _42 = translation.y;    _43 = translation.z;    _44 = 1;
}

void Matrix::SetIdentity()
{
    _11 = 1; _12 = 0; _13 = 0; _14 = 0;
    _21 = 0; _22 = 1; _23 = 0; _24 = 0;
    _31 = 0; _32 = 0; _33 = 1; _34 = 0;
    _41 = 0; _42 = 0; _43 = 0; _44 = 1;
}

const Vector3 Matrix::GetScale()
{
    int sign_x = Math::Sign(_11 * _12 * _13 * _14) < 0 ? -1 : 1;
    int sign_y = Math::Sign(_21 * _22 * _23 * _24) < 0 ? -1 : 1;
    int sign_z = Math::Sign(_31 * _32 * _33 * _34) < 0 ? -1 : 1;

    return Vector3
    (
        static_cast<float>(sign_x) * sqrtf(powf(_11, 2) + powf(_12, 2) + powf(_13, 2)),
        static_cast<float>(sign_y) * sqrtf(powf(_21, 2) + powf(_22, 2) + powf(_23, 2)),
        static_cast<float>(sign_z) * sqrtf(powf(_31, 2) + powf(_32, 2) + powf(_33, 2))
    );
}

const Quaternion Matrix::GetRotation()
{
    auto scale = GetScale();

    if (scale.x == 0.0f || scale.y == 0.0f || scale.z == 0.0f)
        return Quaternion::Identity;

    float factorX = 1.0f / scale.x;
    float factorY = 1.0f / scale.y;
    float factorZ = 1.0f / scale.z;

    Matrix mat;
    mat._11 = _11 * factorX; mat._12 = _12 * factorX; mat._13 = _13 * factorX; mat._14 = 0.0f;
    mat._21 = _21 * factorY; mat._22 = _22 * factorY; mat._23 = _23 * factorY; mat._24 = 0.0f;
    mat._31 = _31 * factorZ; mat._32 = _32 * factorZ; mat._33 = _33 * factorZ; mat._34 = 0.0f;
    mat._41 = 0.0f; mat._42 = 0.0f; mat._43 = 0.0f; mat._44 = 1.0f;

    return RotationMatrixToQuaternion(mat);
}

void Matrix::Decompose(Vector3 & scale, Quaternion & rotation, Vector3 & translation)
{
    scale       = GetScale();
    rotation    = GetRotation();
    translation = GetTranslation();
}

Vector3 Matrix::operator*(const Vector3 & rhs) const
{
	Vector4 temp;

	temp.x = (rhs.x * _11) + (rhs.y * _21) + (rhs.z * _31) + _41;
	temp.y = (rhs.x * _12) + (rhs.y * _22) + (rhs.z * _32) + _42;
	temp.z = (rhs.x * _13) + (rhs.y * _23) + (rhs.z * _33) + _43;
	temp.w = 1 / ((rhs.x * _14) + (rhs.y * _24) + (rhs.z * _34) + _44);

	return Vector3(temp.x * temp.w, temp.y * temp.w, temp.z * temp.w);
}

const Matrix Matrix::operator*(const Matrix & rhs) const
{
    return Matrix
    (
        _11 * rhs._11 + _12 * rhs._21 + _13 * rhs._31 + _14 * rhs._41,
        _11 * rhs._12 + _12 * rhs._22 + _13 * rhs._32 + _14 * rhs._42,
        _11 * rhs._13 + _12 * rhs._23 + _13 * rhs._33 + _14 * rhs._43,
        _11 * rhs._14 + _12 * rhs._24 + _13 * rhs._34 + _14 * rhs._44,
        _21 * rhs._11 + _22 * rhs._21 + _23 * rhs._31 + _24 * rhs._41,
        _21 * rhs._12 + _22 * rhs._22 + _23 * rhs._32 + _24 * rhs._42,
        _21 * rhs._13 + _22 * rhs._23 + _23 * rhs._33 + _24 * rhs._43,
        _21 * rhs._14 + _22 * rhs._24 + _23 * rhs._34 + _24 * rhs._44,
        _31 * rhs._11 + _32 * rhs._21 + _33 * rhs._31 + _34 * rhs._41,
        _31 * rhs._12 + _32 * rhs._22 + _33 * rhs._32 + _34 * rhs._42,
        _31 * rhs._13 + _32 * rhs._23 + _33 * rhs._33 + _34 * rhs._43,
        _31 * rhs._14 + _32 * rhs._24 + _33 * rhs._34 + _34 * rhs._44,
        _41 * rhs._11 + _42 * rhs._21 + _43 * rhs._31 + _44 * rhs._41,
        _41 * rhs._12 + _42 * rhs._22 + _43 * rhs._32 + _44 * rhs._42,
        _41 * rhs._13 + _42 * rhs._23 + _43 * rhs._33 + _44 * rhs._43,
        _41 * rhs._14 + _42 * rhs._24 + _43 * rhs._34 + _44 * rhs._44
    );
}

const bool Matrix::operator==(const Matrix & rhs) const
{
    const float* lhs_ptr = *this;
    const float* rhs_ptr = rhs;

    for (UINT i = 0; i < 16; i++)
    {
        if (!Math::Equals(lhs_ptr[i], rhs_ptr[i]))
            return false;
    }
    return true;
}
