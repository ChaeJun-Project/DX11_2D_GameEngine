#include "stdafx.h"
#include "SceneManager.h"

#include "RenderManager.h"

#include "Scene.h"
#include "GameObject.h"

#include "Transform.h"
#include "Camera.h"

//GameLogic
#include "GameLogic_Script.h"

SceneManager::~SceneManager()
{
	m_p_current_scene.reset();
}

void SceneManager::Initialize()
{
	m_p_current_scene = std::make_shared<Scene>();

	//Camera(0)
	auto camera = new GameObject();
	camera->SetObjectName("Main Camera");
	camera->SetObjectTag("Main Camera");
	camera->AddComponent(new Transform());
	camera->AddComponent(new Camera());

	camera->GetComponent<Camera>()->SetMainCamera();
	camera->GetComponent<Transform>()->SetTranslation(Vector3(0.0f, 0.0f, -5.0f));

	m_p_current_scene->AddGameObject(camera, 0, true);

	//Particle
	auto particle = new GameObject();
	particle->SetObjectName("Particle");
	particle->SetObjectTag("Particle");
	particle->AddComponent(new Transform());
	particle->AddComponent(new ParticleSystem());

	auto particle_system = particle->GetComponent<ParticleSystem>();
	auto resource_manager = ResourceManager::GetInstance();

	//Set Particle Texture
	particle_system->SetParticleTexture(resource_manager->GetTexture("rain_particle"));
	
	//Set Compute Shader
	auto compute_shader = resource_manager->GetShaderResource(ShaderResourceType::Particle)->GetShader<ComputeShader>();
	particle_system->SetComputeShader(compute_shader);

	//Set Particle Activable Count
	particle_system->SetParticleActivableCount(10);

	//Set Particle Count
	particle_system->SetMaxParticleCount(100);
	
	//Set Particle Spawn
	particle_system->SetParticleSpawnRange(Vector3::Zero);

	//Set Particle Scale
	particle_system->SetParticleScale(Vector3::Zero, Vector3::Zero);

	//Set Particle Color
	particle_system->SetParticleColor(Color4::White, Color4::White);
	
	//Set Particle Speed
	particle_system->SetParticleSpeed(0.0f);

	//Set Particle Life
	particle_system->SetParticleLife(0.0f, 0.0f);
	
	m_p_current_scene->AddGameObject(particle, 1, true);

	//Game Logic
	/*auto game_logic = new GameObject();
	game_logic->SetObjectName("Game Logic");
	game_logic->SetObjectTag("Game Logic");
	game_logic->AddComponent(new Transform());
	game_logic->AddComponent(new GameLogic_Script());

	m_p_current_scene->AddGameObject(game_logic, 1, false);

	game_logic->GetComponent<Script>()->Initialize();*/
}

void SceneManager::Update()
{
	m_p_current_scene->Update();
	m_p_current_scene->LateUpdate();
	m_p_current_scene->FinalUpdate();

	//Update Collisio Manager
	CollisionManager::GetInstance()->Update();
}

void SceneManager::CreatePrefab(GameObject* p_game_object)
{
	p_game_object->RegisterPrefab();
}
