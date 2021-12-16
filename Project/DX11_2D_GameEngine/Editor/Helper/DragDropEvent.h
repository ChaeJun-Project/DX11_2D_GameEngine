#pragma once
#include "stdafx.h"

enum class PayLoadType : UINT
{
	NONE,
	GameObject,
	Prefab,
	Texture,
	Audio,
};

struct PayLoad final
{
	PayLoadType type;
	std::variant<DWORD_PTR, std::string> data;
};

class DragDropEvent final
{
public:
	static void CreateDragDropPayLoad(const PayLoad& pay_load)
	{
		ImGui::SetDragDropPayload
		(
			(const char*)(&pay_load.type),
			(const void*)(&pay_load),
			sizeof(pay_load)
		);
	}

	static PayLoad* ReceiveDragDropPayLoad(const PayLoadType& type)
	{
		//드래그 후 특정 위치에 드랍했을 때
		const auto pay_load = ImGui::AcceptDragDropPayload((const char*)(&type));
		if (pay_load != nullptr)
			return static_cast<PayLoad*>(pay_load->Data);

		else
			return nullptr;
	}
};