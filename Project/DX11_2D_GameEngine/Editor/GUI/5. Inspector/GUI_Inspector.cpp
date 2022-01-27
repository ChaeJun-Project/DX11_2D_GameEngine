#include "stdafx.h"
#include "GUI_Inspector.h"

//Helper
#include "Helper/EditorHelper.h"
#include "Helper/IconProvider.h"

//ItemList
#include "GUI/Module/ItemList/GUI_ItemList.h"

//Component
#include "Component/GUI_Component.h"
#include "Component/1. Transform/GUI_Transform.h"
#include "Component/2. Camera/GUI_Camera.h"
#include "Component/3. SpriteRenderer/GUI_SpriteRenderer.h"
#include "Component/4. Animator2D/GUI_Animator2D.h"
//Animator
#include "Component/6. Collider2D/GUI_Collider2D.h"
#include "Component/7. Light2D/GUI_Light2D.h"
#include "Component/8. ParticleSystem/GUI_ParticleSystem.h"
#include "Component/9. TileMapRenderer/GUI_TileMapRenderer.h"
//RigidBody2D
#include "Component/11. AudioListener/GUI_AudioListener.h"
#include "Component/12. AudioSource/GUI_AudioSource.h"
#include "Component/13. Script/GUI_Script.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>
//Script
#include <Script_Lib/ScriptManager.h>

//Define
#define GAME_OBJECT_NAME_WIDTH		200.0f

#define TAG_WIDTH					120.0f
#define ADD_TAG_WIDTH               90.0f
#define LAYER_WIDTH					120.0f
#define INPUT_TEXT_WIDTH            150.0f

#define ADD_COMPONENT_BUTTON_WIDTH	120.0f

//RigidBody2D는 구현 후 추가

GUI_Inspector::GUI_Inspector(const std::string& inspector_title)
	:IGUI(inspector_title)
{
	m_p_tag_list = std::make_unique<GUI_ItemList>();
	m_p_layer_list = std::make_unique<GUI_ItemList>();
}

GUI_Inspector::~GUI_Inspector()
{
	//GameObject
	m_p_selected_game_object = nullptr;

	//Resource
	m_p_select_resource = nullptr;

	//Tag Clear
	m_p_tag_list.reset();
	m_tag_deque.clear();
	m_tag_deque.shrink_to_fit();

	//Layer Clear
	m_p_layer_list.reset();
	m_layer_map.clear();

	for (auto& gui_component : m_component_gui_map)
	{
		if (gui_component.second != nullptr)
			gui_component.second.reset();
	}

	m_component_gui_map.clear();
}

void GUI_Inspector::Initialize()
{
	//Initialize Tag
	InitializeTag();

	//Initialize Layer
	InitializeLayer();

	//Transform
	m_component_gui_map.insert(std::make_pair(ComponentType::Transform, std::make_unique<GUI_Transform>("Transform")));
	//Camera
	m_component_gui_map.insert(std::make_pair(ComponentType::Camera, std::make_unique<GUI_Camera>("Camera")));
	//SpriteRenderer
	m_component_gui_map.insert(std::make_pair(ComponentType::SpriteRenderer, std::make_unique<GUI_SpriteRenderer>("Sprite Renderer")));
	//Animator2D
	m_component_gui_map.insert(std::make_pair(ComponentType::Animator2D, std::make_unique<GUI_Animator2D>("Animator2D")));
	//Animator
	//m_component_gui_map.insert(std::make_pair(ComponentType::Animator, std::make_unique<GUI_Animator>("Animator")));
	//Collider2D
	m_component_gui_map.insert(std::make_pair(ComponentType::Collider2D, std::make_unique<GUI_Collider2D>("Collider2D")));
	//Light2D
	m_component_gui_map.insert(std::make_pair(ComponentType::Light2D, std::make_unique<GUI_Light2D>("Light2D")));
	//ParticleSystem
	m_component_gui_map.insert(std::make_pair(ComponentType::ParticleSystem, std::make_unique<GUI_ParticleSystem>("ParticleSystem")));
	//TileMapRenderer
	m_component_gui_map.insert(std::make_pair(ComponentType::TileMapRenderer, std::make_unique<GUI_TileMapRenderer>("TileMapRenderer")));
	//RigidBody2D
	//AudioListener
	m_component_gui_map.insert(std::make_pair(ComponentType::AudioListener, std::make_unique<GUI_AudioListener>("Audio Listener")));
	//AudioSource
	m_component_gui_map.insert(std::make_pair(ComponentType::AudioSource, std::make_unique<GUI_AudioSource>("Audio Source")));

	//Script 보류
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
	m_p_selected_game_object = EditorHelper::GetInstance()->GetSelectedGameObject();

	if (m_p_selected_game_object != nullptr)
	{
		if (m_is_show_tag_and_layer_list)
			ShowTagAndLayerList();

		else
		{
			ShowGameObjectInfo();
			ShowAddComponent();
		}
	}

	else
	{
		m_p_select_resource = EditorHelper::GetInstance()->GetSelectedResource();

		if (m_p_select_resource != nullptr)
			ShowResourceInfo();
	}
}

//TODO: 컴포넌트가 추가될 때마다 for문의 범위 변경
void GUI_Inspector::ShowGameObjectInfo()
{
	if (m_p_selected_game_object->IsDead())
		return;

	//GameObject Name, Tag, Layer GUI
	//Object Icon 
	IconProvider::GetInstance()->CreateImage(IconType::Inspector_GameObject, ImVec2(32.0f, 32.0f));
	ImGui::SameLine();

	std::string game_object_name = m_p_selected_game_object->GetGameObjectName();

	ImGui::BeginGroup();
	//Render Check
	std::string label_str = "##" + m_p_selected_game_object->GetGameObjectName() + "_is_active";
	bool is_active = m_p_selected_game_object->IsActive();
	if (ImGui::Checkbox(label_str.c_str(), &is_active))
	{
		m_p_selected_game_object->SetGameObjectActive(is_active);
	}
	ImGui::SameLine();

	//Name
	label_str = "##" + m_p_selected_game_object->GetGameObjectName();
	ImGui::PushItemWidth(GAME_OBJECT_NAME_WIDTH);
	if (ImGui::InputText(label_str.c_str(), &game_object_name, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		m_p_selected_game_object->SetGameObjectName(game_object_name); //Game Object의 이름을 수정한 경우에만 수행
	}
	ImGui::PopItemWidth();

	//Tag
	ShowComboTags();
	ImGui::SameLine();

	//Layer
	ShowComboLayers();
	ImGui::SameLine();

	//Plus Button
	if (IconProvider::GetInstance()->CreateImageButton(IconType::Component_Plus, ImVec2(13.0f, 13.0f)))
	{
		m_is_show_tag_and_layer_list = true;
	}

	ImGui::EndGroup();

	//Component GUI
	for (UINT i = static_cast<UINT>(ComponentType::Transform); i < static_cast<UINT>(ComponentType::END); ++i)
	{
		if (!m_p_selected_game_object->GetComponent(static_cast<ComponentType>(i)))
		{
			continue;
		}

		m_component_gui_map[static_cast<ComponentType>(i)]->SetGameObject(m_p_selected_game_object);
		m_component_gui_map[static_cast<ComponentType>(i)]->Render();
	}
}

void GUI_Inspector::ShowComboTags()
{
	ImGui::BeginGroup();
	ImGui::Text("Tag");
	ImGui::SameLine();

	//Show Combo
	ImGui::PushItemWidth(TAG_WIDTH);
	{
		int index = 0;

		auto current_tag_name = m_p_selected_game_object->GetGameObjectTag();

		for (auto& tag : m_tag_deque)
		{
			m_p_tag_list->AddItem(tag);

			if (tag == current_tag_name)
				m_p_tag_list->SetCurrentListID(index);

			else
				++index;
		}

		const auto& tag_list_vector = m_p_tag_list->GetItemList();

		if (ImGui::BeginCombo("##Tag", current_tag_name.c_str()))
		{
			for (UINT i = 0; i < static_cast<UINT>(tag_list_vector.size()); ++i)
			{
				const bool is_selected = (*(m_p_tag_list->GetCurrentListID()) == i);
				if (ImGui::Selectable(tag_list_vector[i].c_str(), is_selected))
				{
					m_p_selected_game_object->SetGameObjectTag(tag_list_vector[i]);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		m_p_tag_list->ClearItemList();
	}
	ImGui::PopItemWidth();
	ImGui::EndGroup();
}

void GUI_Inspector::ShowComboLayers()
{
	ImGui::BeginGroup();
	ImGui::Text("Layer");
	ImGui::SameLine();

	//Show Combo
	ImGui::PushItemWidth(LAYER_WIDTH);
	{
		int index = 0;

		auto current_layer = m_p_selected_game_object->GetGameObjectLayer();

		for (auto& layer : m_layer_map)
		{
			if (!layer.second.empty())
				m_p_layer_list->AddItem(layer.second);

			if (layer.first == current_layer)
				m_p_layer_list->SetCurrentListID(index);

			else
				++index;
		}

		const auto& layer_list_vector = m_p_layer_list->GetItemList();

		if (ImGui::BeginCombo("##Layer", m_layer_map.find(current_layer)->second.c_str()))
		{
			for (UINT i = 0; i < static_cast<UINT>(layer_list_vector.size()); ++i)
			{
				const bool is_selected = (*(m_p_layer_list->GetCurrentListID()) == i);
				if (ImGui::Selectable(layer_list_vector[i].c_str(), is_selected))
				{
					m_p_selected_game_object->SetGameObjectLayer(i);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		m_p_layer_list->ClearItemList();
	}
	ImGui::PopItemWidth();
	ImGui::EndGroup();
}

void GUI_Inspector::ShowTagAndLayerList()
{
	//Back Button
	if (IconProvider::GetInstance()->CreateImageButton(IconType::Component_Back, ImVec2(24.0f, 24.0f)))
	{
		m_is_show_tag_and_layer_list = false;
	}

	//Tag List
	if (ImGui::TreeNodeEx("##Tag List", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow, "Tags"))
	{
		for (auto& tag : m_tag_deque)
		{
			std::string tag_str = "##Tag";
			std::string tag_name = tag;

			tag_str += tag_name;
			ImGui::PushItemWidth(INPUT_TEXT_WIDTH);
			if (ImGui::InputText(tag_str.c_str(), &tag_name, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				tag = tag_name;
				SaveTag();
			}
			ImGui::PopItemWidth();
		}
		ImGui::TreePop();
	}

	//Add Tag Button
	if (ImGui::Button("Add Tag", ImVec2(ADD_TAG_WIDTH, 0.0f)))
	{
		m_tag_deque.emplace_back(std::string());
	}

	//Layer List
	if (ImGui::TreeNodeEx("##Layer List", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow, "Layers"))
	{
		for (auto& layer : m_layer_map)
		{
			std::string layer_str = "Layer ";
			layer_str += std::to_string(layer.first);
			ImGui::BulletText(layer_str.c_str());
			ImGui::SameLine();

			layer_str = "##" + layer_str;
			std::string layer_name = layer.second;
			ImGui::PushItemWidth(INPUT_TEXT_WIDTH);
			if (ImGui::InputText(layer_str.c_str(), &layer_name, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				layer.second = layer_name;
				SaveLayer();
			}
			ImGui::PopItemWidth();
		}
		ImGui::TreePop();
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
		if (ImGui::MenuItem("Camera"))
		{
			m_p_selected_game_object->AddComponent(ComponentType::Camera);
		}

		//Sprite Renderer
		if (ImGui::MenuItem("Sprite Renderer"))
		{
			m_p_selected_game_object->AddComponent(ComponentType::SpriteRenderer);
		}

		//Animator
		if (ImGui::BeginMenu("Animator"))
		{
			//Animator2D
			if (ImGui::MenuItem("Animator2D"))
			{
				m_p_selected_game_object->AddComponent(ComponentType::Animator2D);
			}

			ImGui::EndMenu();
		}

		//Collider
		if (ImGui::BeginMenu("Collider"))
		{
			//Collider2D
			if (ImGui::MenuItem("Collider2D"))
			{
				m_p_selected_game_object->AddComponent(ComponentType::Collider2D);
			}

			ImGui::EndMenu();
		}

		//Light
		if (ImGui::BeginMenu("Light"))
		{
			//Light2D
			if (ImGui::MenuItem("Light2D"))
			{
				m_p_selected_game_object->AddComponent(ComponentType::Light2D);
			}

			ImGui::EndMenu();
		}

		//ParticleSystem
		if (ImGui::MenuItem("ParticleSystem"))
		{
			m_p_selected_game_object->AddComponent(ComponentType::ParticleSystem);
		}

		//RigidBody
		if (ImGui::BeginMenu("RigidBody"))
		{
			//RigidBody2D
			if (ImGui::MenuItem("RigidBody2D"))
			{
				
			}

			ImGui::EndMenu();
		}

		//TileMapRenderer
		if (ImGui::MenuItem("TileMapRenderer"))
		{
			m_p_selected_game_object->AddComponent(ComponentType::TileMapRenderer);
		}

		//Audio
		if (ImGui::BeginMenu("Audio"))
		{
			//Audio Listener
			if (ImGui::MenuItem("Audio Listener"))
			{
				m_p_selected_game_object->AddComponent(ComponentType::AudioListener);
			}

			//Audio Source
			if (ImGui::MenuItem("Audio Source"))
			{
				m_p_selected_game_object->AddComponent(ComponentType::AudioSource);
			}

			ImGui::EndMenu();
		}

		//Script
		if (ImGui::BeginMenu("Script"))
		{
			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
}

void GUI_Inspector::InitializeTag()
{
	std::string path;
	path = m_folder_path + m_tag_file_path;

	if (FILE_MANAGER->IsExistFile(path))
		LoadTag();

	else
	{
		m_tag_deque.emplace_back(std::string("Default"));
		m_tag_deque.emplace_back(std::string("Main Camera"));
		m_tag_deque.emplace_back(std::string("Light"));

		SaveTag();
	}
}

void GUI_Inspector::SaveTag()
{
	std::string path;
	path = m_folder_path + m_tag_file_path;

	FILE* p_file = nullptr;

	fopen_s(&p_file, path.c_str(), "wb");

	fprintf(p_file, "[Tag Count]\n");
	int tag_count = static_cast<int>(m_tag_deque.size());
	fprintf(p_file, "%d\n", tag_count);

	fprintf(p_file, "\n");

	fprintf(p_file, "[Tag List]\n");
	for (const auto& tag : m_tag_deque)
		fprintf(p_file, "%s\n", tag.c_str());

	fclose(p_file);
}

void GUI_Inspector::LoadTag()
{
	std::string path;
	path = m_folder_path + m_tag_file_path;

	FILE* p_file = nullptr;

	fopen_s(&p_file, path.c_str(), "rb");

	char char_buffer[256] = { 0 };

	//Tag Count
	FILE_MANAGER->FScanf(char_buffer, p_file);
	int tag_count = 0;
	fscanf_s(p_file, "%d\n", &tag_count);

	//Tag List
	FILE_MANAGER->FScanf(char_buffer, p_file);
	for (int i = 0; i < tag_count; ++i)
	{
		FILE_MANAGER->FScanf(char_buffer, p_file);
		m_tag_deque.emplace_back(std::string(char_buffer));
	}

	fclose(p_file);
}

void GUI_Inspector::InitializeLayer()
{
	std::string path;
	path = m_folder_path + m_tag_layer_path;

	if (FILE_MANAGER->IsExistFile(path))
		LoadLayer();

	else
	{
		m_layer_map.insert(std::make_pair(0, "Default"));

		for (UINT i = 1; i < MAX_LAYER; ++i)
			m_layer_map.insert(std::make_pair(i, ""));

		SaveLayer();
	}
}

void GUI_Inspector::SaveLayer()
{
	std::string path;
	path = m_folder_path + m_tag_layer_path;

	FILE* p_file = nullptr;

	fopen_s(&p_file, path.c_str(), "wb");

	fprintf(p_file, "[Layer List]\n");
	for (const auto& layer : m_layer_map)
		fprintf(p_file, "%d %s\n", layer.first, layer.second.c_str());

	fclose(p_file);
}

void GUI_Inspector::LoadLayer()
{
	std::string path;
	path = m_folder_path + m_tag_layer_path;

	FILE* p_file = nullptr;

	fopen_s(&p_file, path.c_str(), "rb");

	char char_buffer[256] = { 0 };

	//Layer List
	FILE_MANAGER->FScanf(char_buffer, p_file);
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		//Index
		UINT index = 0;
		fscanf_s(p_file, "%d", &index);

		//String
		FILE_MANAGER->FScanf(char_buffer, p_file);
		std::string layer_str(char_buffer);

		//공백 제거
		layer_str.erase(remove(layer_str.begin(), layer_str.end(), ' '), layer_str.end());

		m_layer_map.insert(std::make_pair(index, layer_str));
	}

	fclose(p_file);
}
