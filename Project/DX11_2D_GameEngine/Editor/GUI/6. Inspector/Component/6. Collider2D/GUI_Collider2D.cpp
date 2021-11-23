#include "stdafx.h"
#include "GUI_Collider2D.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/FileManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Collider2D.h>

GUI_Collider2D::GUI_Collider2D(const std::string& collider2D_name)
	:GUI_Component(collider2D_name)
{
}

void GUI_Collider2D::Render()
{
	auto collider2D = m_select_game_object->GetComponent<Collider2D>();

	if (collider2D == nullptr)
		return;

	if (BeginComponent(m_component_gui_name, ComponentType::Collider2D, IconType::Component_Collider2D))
	{
		auto offest_position = collider2D->GetOffsetPosition();
		auto offest_scale = collider2D->GetOffsetScale();

		const auto ShowFloat = [](const std::string& label_tag, const char* label_name, float& value)
		{
			ImGui::Text(label_name);
			ImGui::SameLine();
			ImGui::PushItemWidth(60.0f);
			std::string label_str = "##" + label_tag + "_" + FileManager::ConvertCharToString(label_name);
			ImGui::InputFloat(label_str.c_str(), &value);
			ImGui::PopItemWidth();
		};

		const auto ShowVector2 = [&ShowFloat](const char* label_name, Vector2& value)
		{
			ImGui::BeginGroup(); //하나의 그룹으로 묶기
			ImGui::Text(label_name);
			std::string label_str = FileManager::ConvertCharToString(label_name);
			ImGui::SameLine(80.0f); //같은 라인에서 80만큼 떨어져서 시작

			//Draw X
			ShowFloat(label_str, "X", value.x); ImGui::SameLine(); //같은 라인에 시작
			//Draw Y
			ShowFloat(label_str, "Y", value.y);

			ImGui::EndGroup(); //하나의 그룹으로 묶기 해제
		};

		ShowVector2("Offset", offest_position);
		ShowVector2("Size", offest_scale);

		collider2D->SetOffsetPosition(offest_position);
		collider2D->SetOffsetScale(offest_scale);

		DrawComponentEnd();
	}
}
