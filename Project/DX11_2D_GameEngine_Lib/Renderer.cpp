#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer(GameObject* p_game_object)
	:IComponent(ComponentType::Renderer, p_game_object)
{
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

	m_p_game_object->GetComponent<Transform>()->UpdateConstantBuffer();

	this->m_p_material->BindPipeline();

	this->m_p_mesh->Render();
}
