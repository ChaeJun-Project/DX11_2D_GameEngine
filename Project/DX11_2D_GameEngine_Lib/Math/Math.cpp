#include "stdafx.h"
#include "Math.h"

const bool Math::IntersectTriangle(const Vector3 & origin, const Vector3 & direction, const Vector3 & v0, const Vector3 & v1, const Vector3 & v2, float * t, float * u, float * v)
{
	Vector3 edge1 = v1 - v0;
	Vector3 edge2 = v2 - v0;

	auto p_vec = Vector3::Cross(direction, edge2);
	auto dot = Vector3::Dot(edge1, p_vec);

	Vector3 t_vec;
	if (dot > 0)
		t_vec = origin - v0;
	else
	{
		t_vec = v0 - origin;
		dot = -dot;
	}

	if (dot < 0.0001f)
		return false;

	if (u)
	{
		*u = Vector3::Dot(t_vec, p_vec);
		if (*u < 0.0f || *u > dot)
			return false;
	}

	Vector3 q_vec = Vector3::Cross(t_vec, edge1);

	if (v)
	{
		*v = Vector3::Dot(direction, q_vec);
		if (*v < 0.0f || *u + *v > dot)
			return false;
	}

	if (t)
	{
		*t = Vector3::Dot(edge2, q_vec);
		float inv_dot = 1.0f / dot;
		*t *= inv_dot;
		*u *= inv_dot;
		*v *= inv_dot;
	}

	return true;
}
