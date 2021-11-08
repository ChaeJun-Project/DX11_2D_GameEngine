#include "stdafx.h"
#include "Renderer.h"

#include "Mesh.h"
#include "Material.h"

Renderer::Renderer()
	:IComponent(ComponentType::Renderer)
{
	
	this->m_p_current_material = std::make_shared<Material>("GameObject_Material");

	this->m_p_border = ResourceManager::GetInstance()->GetMaterial("Collider2D_White");
}

Renderer::Renderer(const Renderer& origin)
	: IComponent(origin.GetComponentType())
{
	this->m_p_mesh = origin.m_p_mesh;

	this->m_p_current_material = origin.m_p_current_material;
	this->m_p_shared_material = origin.m_p_shared_material;
	this->m_p_cloned_material = origin.m_p_cloned_material;

	this->m_p_border = origin.m_p_border;
}

Renderer::~Renderer()
{
	this->m_p_mesh.reset();

	this->m_p_current_material.reset();
	this->m_p_shared_material.reset();
	this->m_p_cloned_material.reset();

    this->m_p_border.reset();
}

void Renderer::FinalUpdate()
{
}

void Renderer::Render()
{
	if (this->m_p_mesh == nullptr || this->m_p_current_material == nullptr || this->m_p_current_material->GetShader() == nullptr)
		return;

	m_p_owner_game_object->GetComponent<Transform>()->UpdateConstantBuffer();

	this->m_p_current_material->BindPipeline();

	this->m_p_mesh->Render();

	this->m_p_border->BindPipeline();

	this->m_p_mesh->Render();
}

void Renderer::SetMaterial(const std::shared_ptr<Material>& p_current_material)
{
   this->m_p_shared_material = p_current_material;
   this->m_p_current_material = this->m_p_shared_material;

   if (this->m_p_cloned_material != nullptr)
   {
	   this->m_p_cloned_material.reset();
	   this->m_p_cloned_material = nullptr;
   }
}

std::shared_ptr<Material> Renderer::GetSharedMaterial()
{
	return this->m_p_shared_material;
}

std::shared_ptr<Material> Renderer::GetClonedMaterial()
{
	if (this->m_p_shared_material != nullptr)
	{
		if(this->m_p_cloned_material == nullptr)
			this->m_p_cloned_material = this->m_p_shared_material;
	}

	else
	   return nullptr;

	return this->m_p_cloned_material;
}
