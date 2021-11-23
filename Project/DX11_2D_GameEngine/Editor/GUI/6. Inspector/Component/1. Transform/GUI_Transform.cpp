#include "stdafx.h"
#include "GUI_Transform.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/FileManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>

GUI_Transform::GUI_Transform(const std::string& transform_gui_name)
	:GUI_Component(transform_gui_name)
{
}

void GUI_Transform::Render()
{
	if (BeginComponent(m_component_gui_name, ComponentType::Transform, IconType::Component_Transform))
	{
		auto transform = m_select_game_object->GetComponent<Transform>();

		auto position = transform->GetLocalTranslation();
		auto rotation = transform->GetLocalRotation().ToEulerAngle();
		auto scale = transform->GetLocalScale();

		const auto ShowFloat = [](const std::string& label_tag, const char* label_name, float& value)
		{
			ImGui::Text(label_name);
			ImGui::SameLine();
			ImGui::PushItemWidth(70.0f);
			std::string label_str = "##" + label_tag + "_" + FileManager::ConvertCharToString(label_name);
			ImGui::InputFloat(label_str.c_str(), &value);
			ImGui::PopItemWidth();
		};

		const auto ShowVector3 = [&ShowFloat](const char* label_name, Vector3& value)
		{
			ImGui::BeginGroup(); //하나의 그룹으로 묶기
			ImGui::Text(label_name);
			std::string label_str = FileManager::ConvertCharToString(label_name);
			ImGui::SameLine(80.0f); //같은 라인에서 80만큼 떨어져서 시작

			//Draw X
			ShowFloat(label_str, "X", value.x); ImGui::SameLine(); //같은 라인에 시작
			//Draw Y
			ShowFloat(label_str, "Y", value.y); ImGui::SameLine(); //같은 라인에 시작
			//Draw Z
			ShowFloat(label_str, "Z", value.z);

			ImGui::EndGroup(); //하나의 그룹으로 묶기 해제
		};

		ShowVector3("Position", position);
		ShowVector3("Rotation", rotation);
		ShowVector3("Scale", scale);

		transform->SetTranslation(position);
		transform->SetRotation(Quaternion::QuaternionFromEulerAngle(rotation));
		transform->SetScale(scale);

		DrawComponentEnd();
	}
}
