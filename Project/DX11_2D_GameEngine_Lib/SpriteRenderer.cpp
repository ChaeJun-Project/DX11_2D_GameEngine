#include "stdafx.h"
#include "SpriteRenderer.h"

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
	//m_p_current_material = resource_manager->GetMaterial("Default_Material");
	m_p_mesh = resource_manager->GetMesh(MeshType::Rectangle);
	m_p_current_material = std::make_shared<Material>("GameObject_Material");
	m_p_current_material->SetShader(resource_manager->GetShaderResource(ShaderResourceType::Light2D, "Light2D_Shader"));
	m_p_border = resource_manager->GetMaterial("Collider2D_White");
}

SpriteRenderer::SpriteRenderer(const SpriteRenderer& origin)
	: IComponent(origin.GetComponentType())
{
	m_p_mesh = origin.m_p_mesh;

	m_p_current_material = origin.m_p_current_material;
	m_p_shared_material = origin.m_p_shared_material;
	m_p_cloned_material = origin.m_p_cloned_material;

	m_p_border = origin.m_p_border;
}

SpriteRenderer::~SpriteRenderer()
{
	m_p_mesh.reset();

	m_p_current_material.reset();
	m_p_shared_material.reset();
	m_p_cloned_material.reset();

	m_p_border.reset();
}

void SpriteRenderer::FinalUpdate()
{

}

void SpriteRenderer::Render()
{
	if (m_p_mesh == nullptr || m_p_current_material == nullptr || m_p_current_material->GetShader() == nullptr)
		return;

	Animator2D* animator2D = nullptr;

	animator2D = m_p_owner_game_object->GetComponent<Animator2D>();

	//해당 GameObject가 Animator2D Component를 소유하고 있는 경우
	if (animator2D)
	{
		auto resource_manager = ResourceManager::GetInstance();

		m_p_current_material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, animator2D->GetAtlasTexture());

		if (animator2D->GetCurrentAnimation() != nullptr)
		{
			int flag = 1;
			//Set Has Animation
			m_p_current_material->SetConstantBufferData(Material_Parameter::INT_0, &flag);

			//Set Animator ID
			flag = animator2D->GetAnimator2DID();
			m_p_current_material->SetConstantBufferData(Material_Parameter::INT_1, &flag);
		}
	}

	if (m_p_owner_game_object->GetObjectTag() != "Water")
		m_p_sprite_texture = m_p_current_material->GetTexture();

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

	m_p_current_material->BindPipeline();

	m_p_mesh->Render();

	m_p_border->BindPipeline();

	m_p_mesh->Render();
}

void SpriteRenderer::SetMaterial(const std::shared_ptr<Material>& p_current_material)
{
	m_p_shared_material = p_current_material;
	m_p_current_material = m_p_shared_material;

	if (m_p_cloned_material != nullptr)
	{
		m_p_cloned_material.reset();
		m_p_cloned_material = nullptr;
	}
}

std::shared_ptr<Material> SpriteRenderer::GetSharedMaterial()
{
	return m_p_shared_material;
}

std::shared_ptr<Material> SpriteRenderer::GetClonedMaterial()
{
	if (m_p_shared_material != nullptr)
	{
		if (m_p_cloned_material == nullptr)
			m_p_cloned_material = m_p_shared_material;
	}

	else
		return nullptr;

	return m_p_cloned_material;
}
