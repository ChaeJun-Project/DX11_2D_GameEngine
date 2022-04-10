#include "stdafx.h"
#include "SpriteRenderer.h"

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

#include "GameObject.h"
#include "Transform.h"
#include "Animator2D.h"
#include "SpriteAnimation.h"

SpriteRenderer::SpriteRenderer()
	:IComponent(ComponentType::SpriteRenderer)
{
	auto p_clone_material_raw = RESOURCE_MANAGER->GetResource<Material>("Default_Material")->Clone();
	m_p_material = std::shared_ptr<Material>(p_clone_material_raw);

	m_p_mesh = RESOURCE_MANAGER->GetResource<Mesh>("Rectangle_Mesh");
}

SpriteRenderer::SpriteRenderer(const SpriteRenderer& origin)
	:IComponent(origin.m_component_type)
{
	m_is_active = origin.m_is_active;

	//Sprite Texture
	m_p_sprite_texture = origin.m_p_sprite_texture;
	//Texture Color
	m_sprite_texture_color = origin.m_sprite_texture_color;

	//Material
	auto p_clone_material_raw = origin.m_p_material->Clone();
	m_p_material = std::shared_ptr<Material>(p_clone_material_raw);
	//Mesh
	m_p_mesh = origin.m_p_mesh;
}

SpriteRenderer::~SpriteRenderer()
{
	//Sprite Texture
	m_p_sprite_texture.reset();

	//Material
	m_p_material.reset();
	//Mesh
	m_p_mesh.reset();
}

void SpriteRenderer::Render()
{
	if (m_p_mesh == nullptr || m_p_material == nullptr || m_p_material->GetShader() == nullptr)
		return;

	//Game Play 중이라면
	if (SCENE_MANAGER->GetClientState() == 1 || SCENE_MANAGER->GetEditorState() == EditorState::EditorState_Play)
	{
		if (m_p_owner_game_object->GetGameObjectTag()._Equal("Post Effect"))
			m_p_sprite_texture = RENDER_MANAGER->GetPostEffectRenderTargetTexture();
	}

	SetMeshScale();

	m_p_material->BindPipeline();

	m_p_mesh->Render();
}

void SpriteRenderer::SetMeshScale()
{
	auto p_animator2D = m_p_owner_game_object->GetComponent<Animator2D>();

	//해당 GameObject가 Animator2D Component를 소유하고 있는 경우
	if (p_animator2D != nullptr)
	{
		m_p_sprite_texture = p_animator2D->GetCurrentAnimationAtlasTexture();

		auto current_animation = p_animator2D->GetCurrentAnimation();

		if (current_animation != nullptr)
		{
			int flag = 1;
			//Set Has Animation
			m_p_material->SetConstantBufferData(Material_Parameter::INT_0, &flag);
			current_animation->UpdateConstantBuffer();
		}
	}

	//Set Sprtie Texture Color
	m_p_material->SetConstantBufferData(Material_Parameter::VEC4_0, &m_sprite_texture_color);

	//Set Mesh Scale
	auto transform = m_p_owner_game_object->GetComponent<Transform>();

	if (m_p_sprite_texture != nullptr)
	{
		m_p_material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, m_p_sprite_texture);

		if (p_animator2D && p_animator2D->GetCurrentAnimation() != nullptr)
		{
			auto current_animation = p_animator2D->GetCurrentAnimation();
			auto frame_size = current_animation->GetCurrentFrame().frame_size;
			transform->SetMeshScale(static_cast<UINT>(frame_size.x), static_cast<UINT>(frame_size.y));
		}

		else
			transform->SetMeshScale(m_p_sprite_texture->GetWidth(), m_p_sprite_texture->GetHeight());
	}
	else
		transform->SetMeshScale(0, 0); //설정된 Texture가 없다면 다시 0으로 초기화

	transform->UpdateConstantBuffer(); //Transform Update
}

void SpriteRenderer::SetSpriteTexture(const std::shared_ptr<Texture>& p_sprite_texture)
{
	if (m_p_sprite_texture != nullptr)
		m_p_sprite_texture.reset();

	m_p_sprite_texture = p_sprite_texture;
}

void SpriteRenderer::SetMaterial(const std::shared_ptr<Material>& p_material)
{
	if (m_p_material != nullptr)
		m_p_material.reset();

	m_p_material = p_material;
}

void SpriteRenderer::SetMesh(const std::shared_ptr<Mesh>& p_mesh)
{
	if (m_p_mesh != nullptr)
		m_p_mesh.reset();

	m_p_mesh = p_mesh;
}

void SpriteRenderer::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	//Texture
	fprintf(p_file, "[Texture]\n");
	RESOURCE_MANAGER->SaveResource<Texture>(m_p_sprite_texture, p_file);

	//Sprite Color
	fprintf(p_file, "[Sprite Color]\n");
	FILE_MANAGER->FPrintf_Vector4<Vector4>(m_sprite_texture_color, p_file);

	//Material
	fprintf(p_file, "[Material]\n");
	RESOURCE_MANAGER->SaveResource<Material>(m_p_material, p_file);

	//Mesh
	fprintf(p_file, "[Mesh]\n");
	RESOURCE_MANAGER->SaveResource<Mesh>(m_p_mesh, p_file);
}

void SpriteRenderer::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //IComponent

	char char_buffer[256] = { 0 };

	//Texture
	FILE_MANAGER->FScanf(char_buffer, p_file);
	RESOURCE_MANAGER->LoadResource<Texture>(m_p_sprite_texture, p_file);

	//Sprite Color
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector4<Vector4>(m_sprite_texture_color, p_file);

	//Material
	FILE_MANAGER->FScanf(char_buffer, p_file);
	RESOURCE_MANAGER->LoadResource<Material>(m_p_material, p_file);

	//Mesh
	FILE_MANAGER->FScanf(char_buffer, p_file);
	RESOURCE_MANAGER->LoadResource<Mesh>(m_p_mesh, p_file);
}
