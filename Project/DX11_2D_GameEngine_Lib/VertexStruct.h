#pragma once

//Vertex Buffer�� ���޵� ����ü ����

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

#pragma region VertexColorTexture
struct VertexColorTexture final
{
	VertexColorTexture()
	{
		this->position = Vector3(0.0f, 0.0f, 0.0f);
		this->color = Color4::White;
		this->uv = Vector2(0.0f, 0.0f);
	}

	VertexColorTexture(const Vector3& position, const Color4& color, const Vector2& uv)
	{
		this->position = position;
		this->color = color;
		this->uv = uv;
	}

	Vector3 position;
	Color4 color;
	Vector2 uv;

	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const UINT element_count = 3;
};
#pragma endregion
