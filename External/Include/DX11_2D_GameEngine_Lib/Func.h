#pragma once

#include "FileManager.h"

//ID를 생성해서 반환
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

	return static_cast<UINT>(hasher(FILE_MANAGER->ConvertWStringToString(guid_wstr)));
}