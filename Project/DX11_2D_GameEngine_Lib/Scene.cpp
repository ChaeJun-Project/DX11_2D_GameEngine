#include "stdafx.h"
#include "Scene.h"
#include "GameObject.h"

#include "Transform.h"
#include "Camera.h"
#include "Render.h"
#include "RockManZ_Script.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	for (auto& object : m_object_list)
		object.reset();

	m_object_list.clear();
}

void Scene::Initialize()
{
	auto camera = std::make_shared<GameObject>(GameObjectType::MainCamera);
	camera->AddAndCreateComponent<Transform>();
	camera->AddAndCreateComponent<Camera>();

	camera->GetComponent<Transform>()->SetTranslation(Vector3(0.0f, 0.0f, -5.0f));

	m_object_list.push_back(camera);

	auto rockmanZ = std::make_shared<GameObject>(GameObjectType::RockManZ);
	rockmanZ->AddAndCreateComponent<Transform>();
	rockmanZ->AddAndCreateComponent<Renderer>();
	rockmanZ->AddAndCreateComponent<Animator>();
	rockmanZ->AddAndCreateComponent<RockManZ_Script>();

	rockmanZ->GetComponent<Transform>()->SetTranslation(Vector3(0.0f, 0.0f, 10.0f));

	rockmanZ->GetComponent<Animator>()->Play();

	m_object_list.push_back(rockmanZ);
}

void Scene::Update()
{
	for (auto& object : m_object_list)
		object->Update();
}

void Scene::LateUpdate()
{
	for (auto& object : m_object_list)
		object->LateUpdate();
}

void Scene::FinalUpdate()
{
	for (auto& object : m_object_list)
		object->FinalUpdate();
}

void Scene::Render()
{
    auto g = GraphicsManager::GetInstance();
	g->BeginScene();
	for (auto& object : m_object_list)
		object->Render();

	g->EndScene();
}
