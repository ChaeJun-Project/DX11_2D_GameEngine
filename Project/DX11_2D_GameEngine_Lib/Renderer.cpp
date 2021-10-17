#include "stdafx.h"
#include "Renderer.h"

#include "Mesh.h"
#include "Material.h"

Renderer::Renderer()
	:IComponent(ComponentType::Renderer)
{
	m_p_material = std::make_shared<Material>("_Material");
}

Renderer::Renderer(const Renderer& origin)
	: IComponent(ComponentType::Renderer)
{
	m_p_mesh = origin.m_p_mesh;
	m_p_material = origin.m_p_material;
}

Renderer::~Renderer()
{
	if (this->m_p_mesh != nullptr)
		this->m_p_mesh.reset();

	if (this->m_p_material != nullptr)
		this->m_p_material.reset();
}

void Renderer::FinalUpdate()
{
}

void Renderer::Render()
{
	if (this->m_p_mesh == nullptr || this->m_p_material == nullptr || this->m_p_material->GetShader() == nullptr)
		return;

	m_p_owner_game_object.lock()->GetComponent<Transform>()->UpdateConstantBuffer();

	this->m_p_material->BindPipeline();

	this->m_p_mesh->Render();
}
