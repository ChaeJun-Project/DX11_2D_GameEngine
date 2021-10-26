#pragma once

#include "stdafx.h"

//DX11_2D_GameEngine_Lib���� �����Ǵ� ��� DirectX11 ��ü�� ���
class DX11Obejct
{
public:
	DX11Obejct() { this->m_id = CreateID(); }
	virtual ~DX11Obejct() = default;

public:
	const UINT& GetObjectID() const { return this->m_id; }

public:
    virtual DX11Obejct* Clone() { return nullptr; }

private:
	//DX11 Object�� ���� ID�� �����ؼ� ��ȯ
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

private:
	UINT m_id = 0;
};
