#pragma once

#include "stdafx.h"
#include "FileManager.h"

//DX11_2D_GameEngine_Lib에서 생성되는 모든 DirectX11 객체에 상속
class DX11Obejct
{
public:
	DX11Obejct() { m_id = CreateID(); }
	virtual ~DX11Obejct() = default;

	virtual void SaveToScene(FILE* p_file)
	{
		fprintf(p_file, "%s\n", m_object_name.c_str());
		//FileManager::SaveStringToFile(m_object_name, p_file);
	}

	virtual void LoadFromScene(FILE* p_file)
	{
		char char_buffer[256] = {};

		FileManager::FScanf(char_buffer, p_file);
		m_object_name = std::string(char_buffer);
		//FileManager::LoadStringFromFile(m_object_name, p_file);
	}

public:
	const UINT& GetObjectID() const { return m_id; }

public:
    virtual DX11Obejct* Clone() { return nullptr; }

private:
	//DX11 Object의 고유 ID를 생성해서 반환
	static const UINT CreateID()
	{
		GUID guid;
		auto hResult = CoCreateGuid(&guid);
		assert(SUCCEEDED(hResult));
		if (!SUCCEEDED(hResult))
			return 0;

		wchar_t p_guid_wchar_t[129];

		auto result = StringFromGUID2(guid, p_guid_wchar_t, 128);
		assert(result);
		if (!result)
			return 0;

		std::wstring guid_wstr(p_guid_wchar_t);

		std::hash<std::string> hasher;

		return static_cast<UINT>(hasher(FileManager::ConvertWStringToString(guid_wstr)));
	}

protected:
	UINT m_id = 0;

	std::string m_object_name = "GameObject";
};
