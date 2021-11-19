#include "stdafx.h"
#include "GUI_Inspector.h"

#include "Helper/IconProvider.h"

//Component
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Camera.h>
#include <DX11_2D_GameEngine_Lib/SpriteRenderer.h>
#include <DX11_2D_GameEngine_Lib/Animator.h>
#include <DX11_2D_GameEngine_Lib/Script.h>
#include <DX11_2D_GameEngine_Lib/Collider2D.h>
#include <DX11_2D_GameEngine_Lib/Light2D.h>
#include <DX11_2D_GameEngine_Lib/ParticleSystem.h>
//RigidBody2D�� ���� �� �߰�

GUI_Inspector::GUI_Inspector(const std::string& inspector_title)
	:IGUI(inspector_title)
{
}

void GUI_Inspector::Update()
{
	if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_I))
	{
		m_is_active = !m_is_active;
	}
}



void GUI_Inspector::Render()
{
    //Test
	BeginComponent("Transform", IconType::Component_Transform);
	BeginComponent("Camera", IconType::Component_Camera);
	BeginComponent("SpriteRenderer", IconType::Component_SpriteRenderer);
	BeginComponent("Animator", IconType::Component_Animator);
	BeginComponent("Script", IconType::Component_Script);
	BeginComponent("Collider2D", IconType::Component_Collider2D);
	BeginComponent("Light2D", IconType::Component_Light2D);
	BeginComponent("ParticleSystem", IconType::Component_ParticleSystem);
	BeginComponent("RigidBody2D", IconType::Component_RigidBody2D);
}

const bool GUI_Inspector::BeginComponent(const std::string component_name, const IconType& icon_type)
{
	//Component Box�� Title �����
	const bool collapsed_header = ImGui::CollapsingHeader(component_name.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen); //â�� Ŵ�� ���ÿ� CollapsingHeader�� ������ Open
	if(collapsed_header)
	{
		ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 8.0f); //���۳�Ʈ ����� ���� ����

		auto icon_provider = IconProvider::GetInstance();

		//Component Settings �׸���
		icon_provider->CreateImageButton(IconType::Component_Settings, ImVec2(13.0f, 13.0f));

		//Component Icon �׸���
		icon_provider->CreateImage(icon_type, ImVec2(14.0f, 14.0f));

		ImGui::SameLine(); //���۳�Ʈ ����� ���� ����

		//Component �̸�
		ImGui::Text(component_name.c_str());

		DrawComponentEnd();
	}

	return collapsed_header;
}

void GUI_Inspector::ShowComponentSettingPopup()
{
}

void GUI_Inspector::DrawComponentEnd()
{
	ImGui::Separator();  //���۳�Ʈ�� ������ ���� �� �׸���
}

void GUI_Inspector::ShowTransformComponent(const Transform* transform)
{
}

void GUI_Inspector::ShowCameraComponent(const Transform* transform)
{
}

void GUI_Inspector::ShowSpriteRenderer(const SpriteRenderer* sprite_renderer)
{
}

void GUI_Inspector::ShowAnimaterComponent(const Animater* animator)
{
}

void GUI_Inspector::ShowScriptComponent(const Script* script)
{
}

void GUI_Inspector::ShowCollider2DComponent(const Collider2D* transform)
{
}

void GUI_Inspector::ShowLight2DComponent(const Light2D* transform)
{
}

void GUI_Inspector::ShowParticleSystemComponent(const ParticleSystem* transform)
{
}

void GUI_Inspector::ShowAddComponent()
{
}

void GUI_Inspector::ShowAddComponentPopup()
{
}
