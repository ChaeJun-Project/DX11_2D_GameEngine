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
	material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, resource_manager->LoadTexture("Texture/Geometry/Forest.gif", TextureType::Standard));
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
	material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, ResourceManager::GetInstance()->LoadTexture("Texture/Geometry/Ground.gif", TextureType::Standard));
	texture = resource_manager->GetTexture("Ground");
	renderer->SetMesh(resource_manager->GetMesh(MeshType::Rectangle));
	
	auto collider2D = ground->GetComponent<Collider2D>();
	collider2D->SetOffsetPosition(Vector2(0.0f, -0.14f));
	collider2D->SetOffsetScale(Vector2(2.6f, 0.34f));
	ground->GetComponent<Transform>()->SetScale(Vector3(2.0f, 2.0f, 1.0f));
	scene_manager->CreatePrefab(ground);
	
	auto ground_prefab = ResourceManager::GetInstance()->GetPrefab("Forest_Ground");

	for (int i = 0; i < 3; ++i)
	{
		auto ground = Instantiate(ground_prefab, Vector3((-1 + i)*(520.0f), -200.0f, 0.0f) , 2, false);
		m_p_owner_game_object->AddChild(ground);
	}

	//Particle
	//auto particle = new GameObject();
	//particle->SetObjectName("Particle");
	//particle->SetObjectTag("Particle");
	//particle->AddComponent(new Transform());
	//particle->AddComponent(new ParticleSystem());

	//auto particle_system = particle->GetComponent<ParticleSystem>();

	////Set Particle Texture
	//particle_system->SetParticleTexture(resource_manager->GetTexture("rain_particle"));

	////Set Compute Shader
	//auto compute_shader = resource_manager->GetShaderResource(ShaderResourceType::Particle, "Particle_Shader")->GetShader<ComputeShader>();
	//particle_system->SetComputeShader(compute_shader);

	////Set Particle Activable Count
	//particle_system->SetParticleActivableCount(50);

	////Set Particle Count
	//particle_system->SetMaxParticleCount(1000);

	////Set Particle Spawn
	//particle_system->SetParticleSpawnRange(Vector3(g_cbuffer_program.resolution.x, g_cbuffer_program.resolution.y, 0.0f));

	////Set Particle Scale
	//particle_system->SetParticleScale(Vector3(10.0f, 30.0f, 1.0f), Vector3(2.0f, 6.0f, 1.0f));

	////Set Particle Color
	//particle_system->SetParticleColor(Color4::White, Color4::White);

	////Set Particle Speed
	//particle_system->SetParticleSpeed(200.0f, 300.0f);

	////Set Particle Life
	//particle_system->SetParticleDrawFrequency(0.4f);

	//particle_system->Initialize();

	//m_p_owner_game_object->AddChild(particle);

	//Water Distortion
	auto water_distortion = new GameObject();
	water_distortion->SetObjectName("Water");
	water_distortion->SetObjectTag("Water");
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
