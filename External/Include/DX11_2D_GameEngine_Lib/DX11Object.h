#pragma once

#include "stdafx.h"

//DX11_2D_GameEngine_Lib���� �����Ǵ� ��� DirectX11 ��ü�� ���
class DX11Obejct
{
public:
	DX11Obejct() { this->m_id = CreateID(); }
	virtual ~DX11Obejct() = default;

	const UINT& GetObjectID() { return this->m_id; }

protected:
	UINT m_id;
};