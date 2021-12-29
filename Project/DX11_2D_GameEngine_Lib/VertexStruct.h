#pragma once

//Vertex Buffer로 전달될 구조체 정의

#pragma region VertexColor
struct VertexColor final
{
	VertexColor()
	{
		m_position = Vector3(0.0f, 0.0f, 0.0f);
		m_color = Vector4::White;
	}

	VertexColor(const Vector3& position, const Vector4& color)
	{
		m_position = position;
		m_color = color;
	}

	Vector3 m_position;
	Vector4 m_color;

	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const UINT element_count = 2;
};
#pragma endregion

#pragma region VertexColorTexture
struct VertexColorTexture final
{
	VertexColorTexture()
	{
		m_position = Vector3(0.0f, 0.0f, 0.0f);
		m_color = Vector4::White;
		m_uv = Vector2(0.0f, 0.0f);
	}

	VertexColorTexture(const Vector3& position, const Vector4& color, const Vector2& uv)
	{
		m_position = position;
		m_color = color;
		m_uv = uv;
	}

	Vector3 m_position;
	Vector4 m_color;
	Vector2 m_uv;

	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const UINT element_count = 3;
};
#pragma endregion
