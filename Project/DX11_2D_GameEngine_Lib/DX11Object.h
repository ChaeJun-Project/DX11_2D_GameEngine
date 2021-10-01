#pragma once

#include "stdafx.h"

//DX11_2D_GameEngine_Lib에서 생성되는 모든 DirectX11 객체에 상속
class DX11Obejct
{
public:
	DX11Obejct() { this->m_id = CreateID(); }
	virtual ~DX11Obejct() = default;

	const UINT& GetObjectID() { return this->m_id; }

protected:
	UINT m_id;
};