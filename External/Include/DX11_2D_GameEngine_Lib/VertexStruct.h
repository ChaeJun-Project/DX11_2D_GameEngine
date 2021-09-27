#pragma once

//Vertex Buffer로 전달될 구조체 정의

#pragma region VertexColor
struct VertexColor final
{
	VertexColor()
	{
		this->position = Vector3(0.0f, 0.0f, 0.0f);
		this->color = Color4::White;
	}

	VertexColor(const Vector3& position, const Color4& color)
	{
		this->position = position;
		this->color = color;
	}

	Vector3 position;
	Color4 color;

	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const UINT element_count = 2;
};
#pragma endregion

#pragma region VertexTexture
struct VertexTexture final
{
	VertexTexture()
	{
		this->position = Vector3(0.0f, 0.0f, 0.0f);
		this->uv = Vector2(0.0f, 0.0f);
	}

	VertexTexture(const Vector3& position, const Vector2& uv)
	{
		this->position = position;
		this->uv = uv;
	}

	Vector3 position;
	Vector2 uv;

	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const UINT element_count = 2;
};
#pragma endregion
