#include "stdafx.h"
#include "SpriteRenderer.h"

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

#include "Transform.h"

SpriteRenderer::SpriteRenderer()
	:IComponent(ComponentType::SpriteRenderer)
{
	m_p_current_material = std::make_shared<Material>("GameObject_Material");

	m_p_border = ResourceManager::GetInstance()->GetMaterial("Collider2D_White");
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

	if (m_p_owner_game_object->GetObjectTag() != "Water")
		m_p_sprite_texture = m_p_current_material->GetTexture();

	auto transform = m_p_owner_game_object->GetComponent<Transform>();
	if (m_p_sprite_texture != nullptr)
	{
		transform->SetMeshScale(m_p_sprite_texture->GetWidth(), m_p_sprite_texture->GetHeight());
	}
	transform->UpdateConstantBuffer();

	m_p_current_material->BindPipeline();

	m_p_mesh->Render();

	if (m_p_owner_game_object->GetObjectTag() != "Water")
	{
		m_p_border->BindPipeline();

		m_p_mesh->Render();
	}
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
