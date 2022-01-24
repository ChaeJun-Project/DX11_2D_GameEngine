#include "stdafx.h"
#include "SpriteRenderer.h"

#include "FileManager.h"
#include "ResourceManager.h"

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

#include "Transform.h"

#include "Animator2D.h"
#include "Animation2D.h"

SpriteRenderer::SpriteRenderer()
	:IComponent(ComponentType::SpriteRenderer)
{
	auto resource_manager = ResourceManager::GetInstance();

	auto clone_material = resource_manager->GetResource<Material>("Default_Material")->Clone();
	m_p_material = std::shared_ptr<Material>(clone_material);

	m_p_mesh = resource_manager->GetResource<Mesh>("Rectangle_Mesh");
}

SpriteRenderer::~SpriteRenderer()
{
	m_p_mesh.reset();
	m_p_material.reset();
}

void SpriteRenderer::Render()
{
	if (m_p_mesh == nullptr || m_p_material == nullptr || m_p_material->GetShader() == nullptr)
		return;

	Animator2D* animator2D = nullptr;

	animator2D = m_p_owner_game_object->GetComponent<Animator2D>();

	//해당 GameObject가 Animator2D Component를 소유하고 있는 경우
	if (animator2D)
	{
		auto resource_manager = ResourceManager::GetInstance();

		m_p_material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, animator2D->GetAtlasTexture());

		if (animator2D->GetCurrentAnimation() != nullptr)
		{
			int flag = 1;
			//Set Has Animation
			m_p_material->SetConstantBufferData(Material_Parameter::INT_0, &flag);

			//Set Animator ID
			flag = animator2D->GetAnimator2DID();
			m_p_material->SetConstantBufferData(Material_Parameter::INT_1, &flag);
		}
	}

	//Set Sprtie Texture Color
	m_p_material->SetConstantBufferData(Material_Parameter::VEC4_0, &m_sprite_texture_color);

	/*if (m_p_owner_game_object->GetGameObjectTag() != "Water")
		m_p_sprite_texture = m_p_material->GetTexture();*/

	auto transform = m_p_owner_game_object->GetComponent<Transform>();
	if (m_p_sprite_texture != nullptr)
	{
		m_p_material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, m_p_sprite_texture);

		if (animator2D && animator2D->GetCurrentAnimation() != nullptr)
		{
			auto current_animation = animator2D->GetCurrentAnimation();
			auto full_frame_size = current_animation->GetCurrentFrameData().full_frame_size;
			transform->SetMeshScale(static_cast<UINT>(full_frame_size.x), static_cast<UINT>(full_frame_size.y));
		}

		else
			transform->SetMeshScale(m_p_sprite_texture->GetWidth(), m_p_sprite_texture->GetHeight());
	}
	transform->UpdateConstantBuffer();

	m_p_material->BindPipeline();

	m_p_mesh->Render();
}

void SpriteRenderer::SetMaterial(const std::shared_ptr<Material>& p_material)
{
	if (m_p_material != nullptr)
		m_p_material.reset();

	m_p_material = p_material;
}

void SpriteRenderer::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	auto resource_manager = ResourceManager::GetInstance();

	//Texture
	fprintf(p_file, "[Texture]\n");
	resource_manager->SaveResource<Texture>(m_p_sprite_texture, p_file);

	//Sprite Color
	fprintf(p_file, "[Sprite Color]\n");
	FileManager::FPrintf_Vector4<Vector4>(m_sprite_texture_color, p_file);

	//Material
	fprintf(p_file, "[Material]\n");
	resource_manager->SaveResource<Material>(m_p_material, p_file);

	//Mesh
	fprintf(p_file, "[Mesh]\n");
	resource_manager->SaveResource<Mesh>(m_p_mesh, p_file);
}

void SpriteRenderer::LoadFromScene(FILE* p_file)
{
	auto resource_manager = ResourceManager::GetInstance();

	char char_buffer[256] = { 0 };

	//Texture
	FileManager::FScanf(char_buffer, p_file);
	resource_manager->LoadResource<Texture>(m_p_sprite_texture, p_file);

	//Sprite Color
	FileManager::FScanf(char_buffer, p_file);
	FileManager::FScanf_Vector4<Vector4>(m_sprite_texture_color, p_file);

	//Material
	FileManager::FScanf(char_buffer, p_file);
	resource_manager->LoadResource<Material>(m_p_material, p_file);

	//Mesh
	FileManager::FScanf(char_buffer, p_file);
	resource_manager->LoadResource<Mesh>(m_p_mesh, p_file);
}
