#include "stdafx.h"
#include "Geometry_Script.h"

#include <DX11_2D_GameEngine_Lib/Scene.h>

#include <DX11_2D_GameEngine_Lib/ResourceManager.h>

#include <DX11_2D_GameEngine_Lib/RenderManager.h>

Geometry_Script::Geometry_Script()
{
}

Geometry_Script::~Geometry_Script()
{
}

void Geometry_Script::Start()
{
	auto scene_manager = SceneManager::GetInstance();
	auto current_scene = scene_manager->GetCurrentScene();

	//BackGround(2)
	auto background = new GameObject();
	background->SetGameObjectName("Forest_Background");
	background->SetGameObjectTag("Background");
	background->AddComponent(new Transform());
	background->AddComponent(new SpriteRenderer());

	auto resource_manager = ResourceManager::GetInstance();

	auto renderer = background->GetComponent<SpriteRenderer>();
	auto material = renderer->GetMaterial();
	material->SetShader(resource_manager->GetResource<Shader>("Light2D"));
	material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, resource_manager->CreateTexture("Texture/Geometry/Forest.gif"));
	auto texture = resource_manager->GetResource<Texture>("Forest");
	renderer->SetMesh(resource_manager->GetResource<Mesh>("Rectangle_Mesh"));

	background->GetComponent<Transform>()->SetScale(Vector3(6.0f, 6.0f, 1.0f));

	m_p_owner_game_object->AddChild(background);

	//Ground(2)
	auto ground = new GameObject();
	ground->SetGameObjectName("Forest_Ground");
	ground->SetGameObjectTag("Ground");
	ground->AddComponent(new Transform());
	ground->AddComponent(new SpriteRenderer());
	ground->AddComponent(new Collider2D());

	renderer = ground->GetComponent<SpriteRenderer>();
	material = renderer->GetMaterial();
	material->SetShader(resource_manager->GetResource<Shader>("Light2D"));
	material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, ResourceManager::GetInstance()->CreateTexture("Texture/Geometry/Ground.gif"));
	texture = resource_manager->GetResource<Texture>("Ground");
	renderer->SetMesh(resource_manager->GetResource<Mesh>("Rectangle_Mesh"));

	auto collider2D = ground->GetComponent<Collider2D>();
	collider2D->SetOffsetPosition(Vector2(0.0f, -0.14f));
	collider2D->SetOffsetScale(Vector2(2.6f, 0.34f));
	ground->GetComponent<Transform>()->SetScale(Vector3(2.0f, 2.0f, 1.0f));
	scene_manager->CreatePrefab(ground);

	auto ground_prefab = ResourceManager::GetInstance()->GetResource<Prefab>("Forest_Ground");

	for (int i = 0; i < 3; ++i)
	{
		auto ground = Instantiate(ground_prefab, Vector3((-1 + i) * (520.0f), -200.0f, 0.0f), 2, false);
		m_p_owner_game_object->AddChild(ground);
	}

	//Particle
	auto particle = new GameObject();
	particle->SetGameObjectName("Particle");
	particle->SetGameObjectTag("Particle");
	particle->AddComponent(new Transform());
	particle->AddComponent(new ParticleSystem());

	auto particle_system = particle->GetComponent<ParticleSystem>();

	//Set Particle Texture
	particle_system->SetParticleTexture(resource_manager->GetResource<Texture>("rain_particle"));

	//Set Compute Shader
	auto compute_shader = resource_manager->GetResource<Shader>("Rain")->GetShader<ComputeShader>();
	particle_system->SetComputeShader(compute_shader);

	//Set Particle Activable Count
	particle_system->SetParticleActivableCount(50);

	//Set Particle Count
	particle_system->SetMaxParticleCount(1000);

	//Set Particle Spawn
	particle_system->SetParticleSpawnRange(Vector3(g_cbuffer_program.resolution.x, g_cbuffer_program.resolution.y, 0.0f));

	//Set Particle Scale
	particle_system->SetParticleScale(Vector3(10.0f, 30.0f, 1.0f), Vector3(2.0f, 6.0f, 1.0f));

	//Set Particle Color
	particle_system->SetParticleColor(Vector4::White, Vector4::White);

	//Set Particle Speed
	particle_system->SetParticleSpeed(200.0f, 300.0f);

	//Set Particle Life
	particle_system->SetParticleDrawFrequency(0.4f);

	particle_system->Start();

	m_p_owner_game_object->AddChild(particle);

	//Water Distortion
	auto water_distortion = new GameObject();
	water_distortion->SetGameObjectName("Water");
	water_distortion->SetGameObjectTag("Water");
	water_distortion->AddComponent(new Transform());
	water_distortion->AddComponent(new SpriteRenderer());

	auto water_transform = water_distortion->GetComponent<Transform>();
	water_transform->SetScale(Vector3(g_cbuffer_program.resolution.x, (g_cbuffer_program.resolution.y * 0.5f) - 240.0f, 1.0f));
	water_transform->SetTranslation(Vector3(0.0f, -(g_cbuffer_program.resolution.y * 0.5f) + 80.0f, 0.0f));

	renderer = water_distortion->GetComponent<SpriteRenderer>();
	renderer->SetMesh(resource_manager->GetResource<Mesh>("Rectangle_Mesh"));

	auto water_material = ResourceManager::GetInstance()->GetResource<Material>("WaterEffect");
	auto water_heigiht = water_transform->GetLocalScale().y;
	water_material->SetConstantBufferData(Material_Parameter::FLOAT_0, &water_heigiht);
	float refract_scale = 0.01f;
	water_material->SetConstantBufferData(Material_Parameter::FLOAT_1, &refract_scale);
	water_material->SetConstantBufferData(Material_Parameter::TEX_1, nullptr, resource_manager->GetResource<Texture>("noise_03"));
	renderer->SetMaterial(resource_manager->GetResource<Material>("WaterEffect"));

	m_p_owner_game_object->AddChild(water_distortion);
}

void Geometry_Script::Update()
{
}

void Geometry_Script::OnCollisionEnter(GameObject* other_game_object)
{
}

void Geometry_Script::OnCollisionExit(GameObject* other_game_object)
{
}

void Geometry_Script::OnCollision(GameObject* other_game_object)
{
}
