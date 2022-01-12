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

	m_p_mesh = resource_manager->GetResource<Mesh>("Rectangle_Mesh");
	auto clone_material = resource_manager->GetResource<Material>("Default_Material")->Clone();
	m_p_material = std::shared_ptr<Material>(clone_material);
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

	if (m_p_owner_game_object->GetGameObjectTag() != "Water")
		m_p_sprite_texture = m_p_material->GetTexture();

	auto transform = m_p_owner_game_object->GetComponent<Transform>();
	if (m_p_sprite_texture != nullptr)
	{
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
    if(m_p_material != nullptr)
		m_p_material.reset();

	m_p_material = p_material;
}

void SpriteRenderer::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	//int null_check = !!m_p_mesh.get();
	//fwrite(&null_check, sizeof(int), 1, p_file);

	//if (null_check)
	//{
	//	FileManager::SaveStringToFile(m_p_mesh->GetResourceName(), p_file);
	//	FileManager::SaveStringToFile(m_p_mesh->GetResourcePath(), p_file);
	//}

	//std::shared_ptr<Material> m_p_shared_material = nullptr; //공유 Material
}

void SpriteRenderer::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //IComponent

	//int null_check = !!m_p_mesh.get();
	//fread(&null_check, sizeof(int), 1, p_file);

	//if (null_check)
	//{
	//	std::string resource_name, resource_path;
	//	FileManager::LoadStringFromFile(resource_name, p_file);
	//	FileManager::LoadStringFromFile(resource_path, p_file);
	//}
	//m_p_mesh = ResourceManager::GetInstance()->CreateMesh();
}
