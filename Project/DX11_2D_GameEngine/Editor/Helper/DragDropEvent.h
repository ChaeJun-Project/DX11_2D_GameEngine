#pragma once
#include "stdafx.h"

enum class PayLoadType : UINT
{
	None = 0,
	GameObject = 1,
	Folder = 2,
	Animation = 3,
	Audio = 4,
	Material = 5,
	Mesh = 6,
	Scene = 7,
	Texture = 8,
	TileMap = 9,
	Prefab = 10,
	Particle = 11,
};

//드래그된 아이템의 데이터
struct PayLoad final
{
	PayLoadType type;
	std::variant<DWORD_PTR, std::string> data; //오브젝트의 주소 또는 리소스 경로를 데이터로 가질 수 있음
};

class DragDropEvent final
{
public:
	static void CreateDragDropPayLoad(const PayLoad& pay_load)
	{
		ImGui::SetDragDropPayload
		(
			reinterpret_cast<const char*>(&pay_load.type),
			reinterpret_cast<const void*>(&pay_load),
			sizeof(pay_load)
		);
	}

	static PayLoad* ReceiveDragDropPayLoad(const PayLoadType& type)
	{
		//Imgui 내에서 DragDrop을 판별
		if (ImGui::BeginDragDropTarget())
		{
			//드래그 후 특정 위치에 드랍했을 때
			if (const auto pay_load = ImGui::AcceptDragDropPayload(reinterpret_cast<const char*>(&type)))
				return static_cast<PayLoad*>(pay_load->Data);

			ImGui::EndDragDropTarget();
		}
		return nullptr;
	}
};