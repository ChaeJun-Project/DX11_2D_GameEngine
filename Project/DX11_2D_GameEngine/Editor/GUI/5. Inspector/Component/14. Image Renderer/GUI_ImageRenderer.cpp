#include "stdafx.h"
#include "GUI_ImageRenderer.h"

#include "GUI/Module/ItemList/GUI_ItemList.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/Texture.h>
#include <DX11_2D_GameEngine_Lib/Mesh.h>
#include <DX11_2D_GameEngine_Lib/Material.h>

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/ImageRenderer.h>

GUI_ImageRenderer::GUI_ImageRenderer(const std::string& image_renderer_gui_name)
	:GUI_Component(image_renderer_gui_name)
{
	m_p_item_list = new GUI_ItemList();

	m_p_image_type_list = new GUI_ItemList();
	m_p_fill_method_type_list = new GUI_ItemList();
	m_p_direction_type_list = new GUI_ItemList();
}

GUI_ImageRenderer::~GUI_ImageRenderer()
{
	SAFE_DELETE(m_p_item_list);

	SAFE_DELETE(m_p_image_type_list);
	SAFE_DELETE(m_p_fill_method_type_list);
	SAFE_DELETE(m_p_direction_type_list);
}

void GUI_ImageRenderer::Render()
{
	auto p_image_renderer = m_p_selected_game_object->GetComponent<ImageRenderer>();
	if (p_image_renderer == nullptr)
		return;

	auto is_active = p_image_renderer->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::ImageRenderer, is_active, IconType::Component_ImageRenderer))
	{
		auto p_image_renderer = m_p_selected_game_object->GetComponent<ImageRenderer>();
		if (p_image_renderer == nullptr)
			return;

		p_image_renderer->SetIsActive(is_active);

		//Image Renderer Property
		//Texture
		ShowTexture("Texture", p_image_renderer);

		//Image Color
		auto image_color = p_image_renderer->GetImageColor();
		ShowColorPicker4("Image Color", image_color, ImGuiColorEditFlags_AlphaPreview);
		if (CAN_EDIT)
			p_image_renderer->SetImageColor(image_color);

		//Material
		ShowMaterialCombo("Material", p_image_renderer);
		//Mesh
		ShowMeshCombo("Mesh", p_image_renderer);

		//Image Property
		auto p_texture = p_image_renderer->GetTexture();
		if (p_texture != nullptr)
			ShowImagePropertyCombo(p_image_renderer);

		DrawComponentEnd();
	}
}

void GUI_ImageRenderer::ShowTexture(const std::string& label_name, ImageRenderer* p_image_renderer)
{
	ImGui::Text(label_name.c_str());
	ImGui::SameLine(100.0f);

	ImGui::PushItemWidth(200.0f);
	auto p_texture = p_image_renderer->GetTexture();
	std::string texture_name;
	if (p_texture != nullptr)
		texture_name = p_texture->GetResourceName();
	std::string texture_name_label = "##" + texture_name;
	ImGui::InputText(texture_name_label.c_str(), &texture_name, ImGuiInputTextFlags_ReadOnly);
	ImGui::PopItemWidth();

	if (auto pay_load = DragDropEvent::ReceiveDragDropPayLoad(PayLoadType::Texture))
	{
		p_image_renderer->SetTexture(RESOURCE_MANAGER->LoadFromFile<Texture>(std::get<std::string>(pay_load->data)));
	}

	ImGui::Image
	(
		p_texture ? p_texture->GetShaderResourceView() : nullptr,
		ImVec2(80.0f, 80.0f),
		ImVec2(0.0f, 0.0f),
		ImVec2(1.0f, 1.0f),
		ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
		ImColor(1.0f, 1.0f, 1.0f, 1.0f)
	);
}

void GUI_ImageRenderer::ShowMaterialCombo(const std::string& label_name, ImageRenderer* p_image_renderer)
{
	ImGui::Text(label_name.c_str());
	ImGui::SameLine(100.0f);

	ImGui::PushItemWidth(200.0f);
	auto p_material = p_image_renderer->GetMaterial();
	std::string material_name;
	if (p_material != nullptr)
		material_name = p_material->GetResourceName();

	int index = 0;

	const auto material_map = RESOURCE_MANAGER->GetResourceMap(ResourceType::Material);

	for (const auto& material : material_map)
	{
		m_p_item_list->AddItem(material.second->GetResourceName());

		if (material.second->GetResourceName()._Equal(material_name))
			m_p_item_list->SetCurrentListID(index);

		else
			++index;
	}

	const auto& item_list_vector = m_p_item_list->GetItemList();

	if (ImGui::BeginCombo("##Material Comobo", material_name.c_str()))
	{
		for (UINT i = 0; i < static_cast<UINT>(item_list_vector.size()); ++i)
		{
			const bool is_selected = (m_p_item_list->GetCurrentListID() == i);
			if (ImGui::Selectable(item_list_vector[i].c_str(), is_selected))
			{
				if (CAN_EDIT)
				{

				}
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	m_p_item_list->ClearItemList();
}

void GUI_ImageRenderer::ShowMeshCombo(const std::string& label_name, ImageRenderer* p_image_renderer)
{
	ImGui::Text(label_name.c_str());
	ImGui::SameLine(100.0f);

	ImGui::PushItemWidth(200.0f);
	auto p_mesh = p_image_renderer->GetMesh();
	std::string mesh_name;
	if (p_mesh != nullptr)
		mesh_name = p_mesh->GetResourceName();

	int index = 0;

	const auto& mesh_map = RESOURCE_MANAGER->GetResourceMap(ResourceType::Mesh);

	for (const auto& mesh : mesh_map)
	{
		m_p_item_list->AddItem(mesh.second->GetResourceName());

		if (mesh.second->GetResourceName()._Equal(mesh_name))
			m_p_item_list->SetCurrentListID(index);

		else
			++index;
	}

	const auto& item_list_vector = m_p_item_list->GetItemList();

	if (ImGui::BeginCombo("##Mesh Combo", mesh_name.c_str()))
	{
		for (UINT i = 0; i < static_cast<UINT>(item_list_vector.size()); ++i)
		{
			const bool is_selected = (m_p_item_list->GetCurrentListID() == i);
			if (ImGui::Selectable(item_list_vector[i].c_str(), is_selected))
			{
				if (CAN_EDIT)
				{

				}
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	m_p_item_list->ClearItemList();
}

void GUI_ImageRenderer::ShowImagePropertyCombo(ImageRenderer* p_image_renderer)
{
	//Image Type
	auto image_type = p_image_renderer->GetImageType();
	ImGui::Text("Image Type");
	ImGui::SameLine(100.0f);

	ImGui::PushItemWidth(150.0f);
	{
		int index = 0;

		std::string current_image_type;

		switch (image_type)
		{
		case ImageType::Default:
			current_image_type = "Default";
			break;
		case ImageType::Filled:
			current_image_type = "Filled";
			break;
		}

		m_p_item_list->AddItem("Default");
		m_p_item_list->AddItem("Filled");

		const auto& item_list_vector = m_p_item_list->GetItemList();

		for (auto& image_type : item_list_vector)
		{
			if (image_type._Equal(current_image_type))
				m_p_item_list->SetCurrentListID(index);

			else
				++index;
		}

		if (ImGui::BeginCombo("##Image Type Combo", current_image_type.c_str()))
		{
			for (UINT i = 0; i < static_cast<UINT>(item_list_vector.size()); ++i)
			{
				const bool is_selected = (m_p_item_list->GetCurrentListID() == i);
				if (ImGui::Selectable(item_list_vector[i].c_str(), is_selected))
				{
					if (CAN_EDIT)
						p_image_renderer->SetImageType(static_cast<ImageType>(i));
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		m_p_item_list->ClearItemList();
	}

	image_type = p_image_renderer->GetImageType();
	switch (image_type)
	{
	case ImageType::Filled:
		ShowFillMethod(p_image_renderer);
		break;
	}
}

void GUI_ImageRenderer::ShowFillMethod(ImageRenderer* p_image_renderer)
{
	auto fill_method = p_image_renderer->GetFillMethod();
	ImGui::Text("Fill Method");
	ImGui::SameLine(100.0f);

	ImGui::PushItemWidth(150.0f);
	{
		int index = 0;

		std::string current_fill_method;

		switch (fill_method)
		{
		case FillMethodType::Horizontal:
			current_fill_method = "Horizontal";
			break;
		case FillMethodType::Vertical:
			current_fill_method = "Vertical";
			break;
		}

		m_p_item_list->AddItem("Horizontal");
		m_p_item_list->AddItem("Vertical");

		const auto& item_list_vector = m_p_item_list->GetItemList();

		for (auto& fill_method : item_list_vector)
		{
			if (fill_method._Equal(current_fill_method))
				m_p_item_list->SetCurrentListID(index);

			else
				++index;
		}

		if (ImGui::BeginCombo("##Fill Method Combo", current_fill_method.c_str()))
		{
			for (UINT i = 0; i < static_cast<UINT>(item_list_vector.size()); ++i)
			{
				const bool is_selected = (m_p_item_list->GetCurrentListID() == i);
				if (ImGui::Selectable(item_list_vector[i].c_str(), is_selected))
				{
					if (CAN_EDIT)
						p_image_renderer->SetFillMethod(static_cast<FillMethodType>(i));
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		m_p_item_list->ClearItemList();
	}

	ShowDirection(p_image_renderer);
}

void GUI_ImageRenderer::ShowDirection(ImageRenderer* p_image_renderer)
{
	ImGui::Text("Direction");
	ImGui::SameLine(100.0f);

	ImGui::PushItemWidth(150.0f);
	{
		int index = 0;

		std::string current_fill_method;

		auto fill_method = p_image_renderer->GetFillMethod();
		switch (fill_method)
		{
		case FillMethodType::Horizontal:
		{
			auto fill_direction = p_image_renderer->GetHorizontalDirection();
			switch (fill_direction)
			{
			case HorizontalDirectionType::Left:
				current_fill_method = "Left";
				break;
			case HorizontalDirectionType::Right:
				current_fill_method = "Right";
				break;
			}

			m_p_item_list->AddItem("Left");
			m_p_item_list->AddItem("Right");
		}
		break;
		case FillMethodType::Vertical:
		{
			auto fill_direction = p_image_renderer->GetVerticalDirection();
			switch (fill_direction)
			{
			case VerticalDirectionType::Bottom:
				current_fill_method = "Bottom";
				break;
			case VerticalDirectionType::Top:
				current_fill_method = "Top";
				break;
			}

			m_p_item_list->AddItem("Bottom");
			m_p_item_list->AddItem("Top");
		}
		break;
		}

		const auto& item_list_vector = m_p_item_list->GetItemList();

		for (auto& fill_method : item_list_vector)
		{
			if (fill_method._Equal(current_fill_method))
				m_p_item_list->SetCurrentListID(index);

			else
				++index;
		}

		if (ImGui::BeginCombo("##Fill Direction Combo", current_fill_method.c_str()))
		{
			for (UINT i = 0; i < static_cast<UINT>(item_list_vector.size()); ++i)
			{
				const bool is_selected = (m_p_item_list->GetCurrentListID() == i);
				if (ImGui::Selectable(item_list_vector[i].c_str(), is_selected))
				{
					if (CAN_EDIT)
					{
						switch (fill_method)
						{
						case FillMethodType::Horizontal:
						{
							p_image_renderer->SetHorizontalDirection(static_cast<HorizontalDirectionType>(i));
						}
						break;
						case FillMethodType::Vertical:
						{
							p_image_renderer->SetVerticalDirection(static_cast<VerticalDirectionType>(i));
						}
						break;
						}
					}
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		m_p_item_list->ClearItemList();
	}

	//Fill Amount
	auto fill_amount = p_image_renderer->GetFillAmount();
	ImGui::Text("Fill Amount");
	ImGui::SameLine(100.0f);

	ImGui::PushItemWidth(150.0f);
	if (ImGui::SliderFloat("##Fill Amount Slider", &fill_amount, 0.0f, 1.0f))
	{
		if (CAN_EDIT)
			p_image_renderer->SetFillAmount(fill_amount);
	}
	ImGui::PopItemWidth();
}