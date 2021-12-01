#include "stdafx.h"
#include "GUI_Inspector.h"

//Helper
#include "Helper/EditorHelper.h"
#include "Helper/IconProvider.h"

//Component
#include "Component/GUI_Component.h"
#include "Component/1. Transform/GUI_Transform.h"
#include "Component/2. Camera/GUI_Camera.h"
#include "Component/3. SpriteRenderer/GUI_SpriteRenderer.h"
#include "Component/4. Animator2D/GUI_Animator2D.h"
//#include "Component/5. Animator/GUI_Animator.h"
#include "Component/6. Script/GUI_Script.h"
#include "Component/7. Collider2D/GUI_Collider2D.h"
#include "Component/8. Light2D/GUI_Light2D.h"
#include "Component/9. ParticleSystem/GUI_ParticleSystem.h"

#define ADD_COMPONENT_BUTTON_WIDTH 120.0f

//RigidBody2D는 구현 후 추가

GUI_Inspector::GUI_Inspector(const std::string& inspector_title)
	:IGUI(inspector_title)
{
	//Transform
	m_component_gui_list.push_back(std::make_pair(ComponentType::Transform, std::make_shared<GUI_Transform>("Transform")));
	//Camera
	m_component_gui_list.push_back(std::make_pair(ComponentType::Camera, std::make_shared<GUI_Camera>("Camera")));
	//SpriteRenderer
	m_component_gui_list.push_back(std::make_pair(ComponentType::SpriteRenderer, std::make_shared<GUI_SpriteRenderer>("Sprite Renderer")));
	//Animator2D
	m_component_gui_list.push_back(std::make_pair(ComponentType::Animator2D, std::make_shared<GUI_Animator2D>("Animator2D")));
	//Animator
	//m_component_gui_list.push_back(std::make_pair(ComponentType::Animator, std::make_shared<GUI_Animator>("Animator")));
	//Script
	m_component_gui_list.push_back(std::make_pair(ComponentType::Script, std::make_shared<GUI_Script>("Script")));
	//Collider2D
	m_component_gui_list.push_back(std::make_pair(ComponentType::Collider2D, std::make_shared<GUI_Collider2D>("Collider2D")));
	//Light2D
	m_component_gui_list.push_back(std::make_pair(ComponentType::Light2D, std::make_shared<GUI_Light2D>("Light2D")));
	//ParticleSystem
	m_component_gui_list.push_back(std::make_pair(ComponentType::ParticleSystem, std::make_shared<GUI_ParticleSystem>("ParticleSystem")));

}

GUI_Inspector::~GUI_Inspector()
{
	for (auto& gui_component : m_component_gui_list)
	{
		if (gui_component.second != nullptr)
			gui_component.second.reset();
	}

	m_component_gui_list.clear();
}

void GUI_Inspector::Update()
{
	if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_I))
	{
		m_is_active = !m_is_active;
	}

	m_select_game_object = EditorHelper::GetInstance()->GetSelectedGameObject();
}

void GUI_Inspector::Render()
{
	ShowGameObjectInfo();

	ShowAddComponent();
}

//TODO: 컴포넌트가 추가될 때마다 for문의 범위 변경
void GUI_Inspector::ShowGameObjectInfo()
{
	if (m_select_game_object == nullptr)
		return;

	//GameObject Name, Tag, Layer GUI
	//Object Icon 
	IconProvider::GetInstance()->CreateImage(IconType::Inspector_GameObject, ImVec2(16.0f, 16.0f));
	ImGui::SameLine();

	std::string game_object_name = m_select_game_object->GetObjectName();
	std::string label_str = "##" + m_select_game_object->GetObjectName();
	//Render Check
	ImGui::Checkbox("", &m_select_game_object->IsActive());
    ImGui::SameLine();
	//Name
	ImGui::PushItemWidth(150.0f);
	if (ImGui::InputText(label_str.c_str(), &game_object_name))
		m_select_game_object->SetObjectName(game_object_name); //Game Object의 이름을 수정한 경우에만 수행
	
	//TODO
	//Tag

    
	//Layer


	//Component GUI
	for (UINT i = static_cast<UINT>(ComponentType::Transform); i <= static_cast<UINT>(ComponentType::ParticleSystem); ++i)
	{
		if (!m_select_game_object->GetComponent(static_cast<ComponentType>(i)))
		{
			continue;
		}

		auto component_gui = GetComponentGUI(static_cast<ComponentType>(i));
		component_gui->SetGameObject(m_select_game_object);
		component_gui->Render();
	}
}

void GUI_Inspector::ShowResourceInfo()
{
}

void GUI_Inspector::ShowAddComponent()
{
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
	ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() * 0.5f - ADD_COMPONENT_BUTTON_WIDTH * 0.5f);
	if (ImGui::Button("Add Component", ImVec2(ADD_COMPONENT_BUTTON_WIDTH, 0.0f)))
		ImGui::OpenPopup("##ComponentPopup");

	ShowAddComponentPopup();
}

void GUI_Inspector::ShowAddComponentPopup()
{
	if (ImGui::BeginPopup("##ComponentPopup"))
	{
		//Camera
		if (ImGui::BeginMenu("Camera"))
		{
			ImGui::EndMenu();
		}

		//Sprite Renderer
		if (ImGui::BeginMenu("Sprite Renderer"))
		{
			ImGui::EndMenu();
		}

		//Animator2D
		if (ImGui::BeginMenu("Animator2D"))
		{
			ImGui::EndMenu();
		}

		//Script
		if (ImGui::BeginMenu("Script"))
		{
			ImGui::EndMenu();
		}

		//Collider2D
		if (ImGui::BeginMenu("Collider2D"))
		{
			ImGui::EndMenu();
		}

		//Light2D
		if (ImGui::BeginMenu("Light2D"))
		{
			ImGui::EndMenu();
		}

		//ParticleSystem
		if (ImGui::BeginMenu("ParticleSystem"))
		{
			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
}

std::shared_ptr<GUI_Component> GUI_Inspector::GetComponentGUI(const ComponentType& component_type) const
{
	for (auto& component_gui : m_component_gui_list)
	{
		if (component_gui.first == component_type)
			return component_gui.second;
	}

	return nullptr;
}

