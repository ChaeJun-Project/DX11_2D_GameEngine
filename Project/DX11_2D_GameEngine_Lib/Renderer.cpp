#include "stdafx.h"
#include "Renderer.h"

#include "Mesh.h"
#include "Material.h"

Renderer::Renderer(GameObject* p_game_object)
	:IComponent(ComponentType::Renderer, p_game_object)
{
	m_p_material = std::make_shared<Material>(p_game_object->GetObjectName() + "_Material");
}

Renderer::~Renderer()
{
	m_p_game_object.reset();

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

	m_p_game_object->GetComponent<Transform>()->UpdateConstantBuffer();

	this->m_p_material->BindPipeline();

	this->m_p_mesh->Render();
}
