#pragma once

#include "stdafx.h"
#include "FileManager.h"

//DX11_2D_GameEngine_Lib���� �����Ǵ� ��� DirectX11 ��ü�� ���
class DX11Obejct
{
public:
	DX11Obejct() { m_id = CreateID(); }
	virtual ~DX11Obejct() = default;

	virtual void SaveToScene(FILE* p_file)
	{
		fprintf(p_file, "%s\n", m_object_name.c_str());
	}

	virtual void LoadFromScene(FILE* p_file)
	{
		char char_buffer[256] = {};

		FILE_MANAGER->FScanf(char_buffer, p_file);
		m_object_name = std::string(char_buffer);
	}

public:
	const UINT& GetObjectID() const { return m_id; }

public:
    virtual DX11Obejct* Clone() { return nullptr; }

protected:
	UINT m_id = 0;

	std::string m_object_name;
};
