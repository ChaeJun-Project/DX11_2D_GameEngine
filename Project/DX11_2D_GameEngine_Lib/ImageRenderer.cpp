#include "stdafx.h"
#include "ImageRenderer.h"

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

#include "GameObject.h"
#include "RectTransform.h"

ImageRenderer::ImageRenderer()
	:IComponent(ComponentType::ImageRenderer)
{
	auto p_clone_material_raw = RESOURCE_MANAGER->GetResource<Material>("ImageRenderer_Material")->Clone();
	m_p_material = std::shared_ptr<Material>(p_clone_material_raw);

	m_p_mesh = RESOURCE_MANAGER->GetResource<Mesh>("Rectangle_Mesh");
}

ImageRenderer::ImageRenderer(const ImageRenderer& origin)
	: IComponent(origin.m_component_type)
{
	m_is_active = origin.m_is_active;

	//Texture
	m_p_texture = origin.m_p_texture;
	//Texture Color
	m_texture_color = origin.m_texture_color;

	//Material
	auto p_clone_material_raw = origin.m_p_material->Clone();
	m_p_material = std::shared_ptr<Material>(p_clone_material_raw);
	//Mesh
	m_p_mesh = origin.m_p_mesh;
}

ImageRenderer::~ImageRenderer()
{
	//Texture
	m_p_texture.reset();

	//Material
	m_p_material.reset();
	//Mesh
	m_p_mesh.reset();
}

void ImageRenderer::Render()
{
	if (m_p_mesh == nullptr || m_p_material == nullptr || m_p_material->GetShader() == nullptr)
		return;

	SetImageRendererInfo();

	m_p_material->BindPipeline();

	m_p_mesh->Render();
}

void ImageRenderer::SetImageRendererInfo()
{
	int has_texture = 0;

	//ImageRenderer에 등록된 Texture가 존재한다면
	if (m_p_texture != nullptr)
	{
	    has_texture = 1;
		//Set Has Texture
		m_p_material->SetConstantBufferData(Material_Parameter::INT_0, &has_texture);
		//Set Texture
		m_p_material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, m_p_texture);

		//Set Image Type
		m_p_material->SetConstantBufferData(Material_Parameter::INT_1, &m_image_type);

		//Set Fill Method
		m_p_material->SetConstantBufferData(Material_Parameter::INT_2, &m_fill_method_type);

		switch (m_fill_method_type)
		{
		case FillMethodType::Horizontal:
			//Set Horizontal Fill Direction
			m_p_material->SetConstantBufferData(Material_Parameter::INT_3, &m_horizontal_direction_type);
			break;
		case FillMethodType::Vertical:
			//Set Vertical Fill Direction
			m_p_material->SetConstantBufferData(Material_Parameter::INT_3, &m_vertical_direction_type);
			break;
		}

		//Set Fill Amount
		m_p_material->SetConstantBufferData(Material_Parameter::FLOAT_0, &m_fill_amount);
	}

	else
		//Set Has Texture
		m_p_material->SetConstantBufferData(Material_Parameter::INT_0, &has_texture);

	//Set Texture Color
	m_p_material->SetConstantBufferData(Material_Parameter::VEC4_0, &m_texture_color);
}

void ImageRenderer::SetTexture(const std::shared_ptr<Texture>& p_texture)
{
	if (m_p_texture != nullptr)
		m_p_texture.reset();

	m_p_texture = p_texture;
}

void ImageRenderer::SetMaterial(const std::shared_ptr<Material>& p_material)
{
	if (m_p_material != nullptr)
		m_p_material.reset();

	m_p_material = p_material;
}

void ImageRenderer::SetMesh(const std::shared_ptr<Mesh>& p_mesh)
{
	if (m_p_mesh != nullptr)
		m_p_mesh.reset();

	m_p_mesh = p_mesh;
}

void ImageRenderer::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	//Texture
	fprintf(p_file, "[Texture]\n");
	RESOURCE_MANAGER->SaveResource<Texture>(m_p_texture, p_file);

	//Color
	fprintf(p_file, "[Color]\n");
	FILE_MANAGER->FPrintf_Vector4<Vector4>(m_texture_color, p_file);

	//Material
	fprintf(p_file, "[Material]\n");
	RESOURCE_MANAGER->SaveResource<Material>(m_p_material, p_file);

	//Mesh
	fprintf(p_file, "[Mesh]\n");
	RESOURCE_MANAGER->SaveResource<Mesh>(m_p_mesh, p_file);

	//Image Type
	fprintf(p_file, "[Image Type]\n");
	auto type = static_cast<UINT>(m_image_type);
	fprintf(p_file, "%d\n", type);

	//Fill Method
	fprintf(p_file, "[Fill Method]\n");
	type = static_cast<UINT>(m_fill_method_type);
	fprintf(p_file, "%d\n", type);

	//Fill Direction
	fprintf(p_file, "[Fill Direction]\n");
	switch (m_fill_method_type)
	{
	case FillMethodType::Horizontal:
		//Set Horizontal Fill Direction
		type = static_cast<UINT>(m_horizontal_direction_type);
		break;
	case FillMethodType::Vertical:
		//Set Vertical Fill Direction
		type = static_cast<UINT>(m_vertical_direction_type);
		break;
	}
	fprintf(p_file, "%d\n", type);

	//Fill Amount
	fprintf(p_file, "[Fill Amount]\n");
	fprintf(p_file, "%f\n", m_fill_amount);
}

void ImageRenderer::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //IComponent

	char char_buffer[256] = { 0 };

	//Texture
	FILE_MANAGER->FScanf(char_buffer, p_file);
	RESOURCE_MANAGER->LoadResource<Texture>(m_p_texture, p_file);

	//Color
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector4<Vector4>(m_texture_color, p_file);

	//Material
	FILE_MANAGER->FScanf(char_buffer, p_file);
	RESOURCE_MANAGER->LoadResource<Material>(m_p_material, p_file);

	//Mesh
	FILE_MANAGER->FScanf(char_buffer, p_file);
	RESOURCE_MANAGER->LoadResource<Mesh>(m_p_mesh, p_file);

	int type = -1;

	//Image Type
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%d\n", &type);
	m_image_type = static_cast<ImageType>(type);

	//Fill Method
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%d\n", &type);
	m_fill_method_type = static_cast<FillMethodType>(type);

	//Fill Direction
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%d\n", &type);
	switch (m_fill_method_type)
	{
	case FillMethodType::Horizontal:
		//Horizontal Fill Direction
		m_horizontal_direction_type = static_cast<HorizontalDirectionType>(type);
		break;
	case FillMethodType::Vertical:
		//Vertical Fill Direction
		m_vertical_direction_type = static_cast<VerticalDirectionType>(type);
		break;
	}
	
	//Fill Amount
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_fill_amount);
}