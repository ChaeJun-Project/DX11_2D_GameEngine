#include "stdafx.h"
#include "Geometry_script.h"

#include "Scene.h"

#include "ResourceManager.h"

#include "RenderManager.h"

Geometry_script::Geometry_script()
{
}

Geometry_script::~Geometry_script()
{
}

void Geometry_script::Initialize()
{
	auto scene_manager = SceneManager::GetInstance();
	auto current_scene = scene_manager->GetCurrentScene();

	//BackGround(2)
	auto background = new GameObject();
	background->SetObjectName("Forest_Background");
	background->SetObjectTag("Background");
	background->AddComponent(new Transform());
	background->AddComponent(new SpriteRenderer());

	auto resource_manager = ResourceManager::GetInstance();

	auto renderer = background->GetComponent<SpriteRenderer>();
	auto material = renderer->GetMaterial();
	material->SetShader(resource_manager->GetShaderResource(ShaderResourceType::Light2D, "Light2D_Shader"));
	material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, resource_manager->LoadTexture("Texture/Geometry/Forest.gif"));
	auto texture = resource_manager->GetTexture("Forest");
	renderer->SetMesh(resource_manager->GetMesh(MeshType::Rectangle));

	background->GetComponent<Transform>()->SetScale(Vector3(6.0f, 6.0f, 1.0f));

	m_p_owner_game_object->AddChild(background);

	//Ground(2)
	auto ground = new GameObject();
	ground->SetObjectName("Forest_Ground");
	ground->SetObjectTag("Ground");
	ground->AddComponent(new Transform());
	ground->AddComponent(new SpriteRenderer());
	ground->AddComponent(new Collider2D());

	renderer = ground->GetComponent<SpriteRenderer>();
	material = renderer->GetMaterial();
	material->SetShader(resource_manager->GetShaderResource(ShaderResourceType::Light2D, "Light2D_Shader"));
	material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, ResourceManager::GetInstance()->LoadTexture("Texture/Geometry/Ground.gif"));
	texture = resource_manager->GetTexture("Ground");
	renderer->SetMesh(resource_manager->GetMesh(MeshType::Rectangle));
	
	auto collider2D = ground->GetComponent<Collider2D>();
	collider2D->SetOffsetPos(Vector3(0.0f, -0.21f, 0.0f));
	collider2D->SetOffsetScale(Vector3(2.6f, 0.35f, 1.0f));
	ground->GetComponent<Transform>()->SetScale(Vector3(2.0f, 2.0f, 1.0f));
	scene_manager->CreatePrefab(ground);
	
	auto ground_prefab = ResourceManager::GetInstance()->GetPrefab("Forest_Ground");

	for (int i = 0; i < 3; ++i)
	{
		auto ground = Instantiate(ground_prefab, Vector3((-1 + i)*(520.0f), -200.0f, 0.0f) , 2, false);
		m_p_owner_game_object->AddChild(ground);
	}

	//Water Distortion
	auto water_distortion = new GameObject();
	water_distortion->SetObjectName("Distortion");
	water_distortion->SetObjectTag("Distortion");
	water_distortion->AddComponent(new Transform());
	water_distortion->AddComponent(new SpriteRenderer());

	auto water_transform = water_distortion->GetComponent<Transform>();
	water_transform->SetScale(Vector3(g_cbuffer_program.resolution.x, (g_cbuffer_program.resolution.y * 0.5f) - 240.0f, 1.0f));
	water_transform->SetTranslation(Vector3(0.0f, -(g_cbuffer_program.resolution.y * 0.5f) + 80.0f, 0.0f));

	renderer = water_distortion->GetComponent<SpriteRenderer>();
	renderer->SetMesh(resource_manager->GetMesh(MeshType::Rectangle));

	auto water_material = ResourceManager::GetInstance()->GetMaterial("WaterEffect");
	auto water_heigiht = water_transform->GetLocalScale().y;
	water_material->SetConstantBufferData(Material_Parameter::FLOAT_0, &water_heigiht);
	float refract_scale = 0.01f;
	water_material->SetConstantBufferData(Material_Parameter::FLOAT_1, &refract_scale);
	water_material->SetConstantBufferData(Material_Parameter::TEX_1, nullptr, resource_manager->GetTexture("noise_03"));
	renderer->SetMaterial(resource_manager->GetMaterial("WaterEffect"));


	m_p_owner_game_object->AddChild(water_distortion);
}

void Geometry_script::Update()
{
}

void Geometry_script::OnCollisionEnter(GameObject* other_game_object)
{
}

void Geometry_script::OnCollisionExit(GameObject* other_game_object)
{
}

void Geometry_script::OnCollision(GameObject* other_game_object)
{
}
