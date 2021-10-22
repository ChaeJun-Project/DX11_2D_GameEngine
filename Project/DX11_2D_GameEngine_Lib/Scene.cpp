#include "stdafx.h"
#include "Scene.h"

#include "Layer.h"
#include "GameObject.h"

#include "Transform.h"
#include "Camera.h"
#include "Renderer.h"
#include "Animation.h"
#include "Collider2D.h"
#include "RockManZ_Script.h"

Scene::~Scene()
{
	for (auto& layer : this->m_layer_map)
		layer.second.reset();

	this->m_layer_map.clear();
}

void Scene::Initialize()
{
	//Camera
	auto camera = new GameObject();
	camera->SetObjectName("Main Camera");
	camera->SetObjectTag("Camera");
	camera->AddComponent(new Transform());
	camera->AddComponent(new Camera());

	camera->GetComponent<Transform>()->SetTranslation(Vector3(0.0f, 0.0f, -5.0f));

	AddGameObject(camera, 0, false);

	//RockManZ
	auto rockmanZ = new GameObject();
	rockmanZ->SetObjectName("RockManZ");
	rockmanZ->SetObjectTag("Player");
	rockmanZ->AddComponent(new Transform());
	rockmanZ->AddComponent(new Renderer());
	rockmanZ->AddComponent(new Animator());
	rockmanZ->AddComponent(new Collider2D());
	rockmanZ->AddComponent(new RockManZ_Script());

	//auto collider2D = rockmanZ->GetComponent<Collider2D>();
	//collider2D->SetOffsetPos(Vector3(0.0f, -10.0f, 0.0f));
	//collider2D->SetOffsetScale(Vector3(0.4f, 0.5f, 1.0f));
	//rockmanZ->GetComponent<Transform>()->SetScale(Vector3(2.0f, 2.0f, 0.0f));
	//auto scale = rockmanZ->GetComponent<Transform>()->GetScale();
	//rockmanZ->GetComponent<Transform>()->SetTranslation(Vector3(0.0f, -(scale.y * 36.0f), 0.0f));

	rockmanZ->GetComponent<Script>()->Initialize();
	rockmanZ->GetComponent<Animator>()->Play();

	AddGameObject(rockmanZ, 1, false);

	//RockManX
	auto rockmanX = new GameObject();
	rockmanX->SetObjectName("RockManX");
	rockmanX->SetObjectTag("Player");
	rockmanX->AddComponent(new Transform());
	rockmanX->AddComponent(new Renderer());
	rockmanX->AddComponent(new Animator());
	rockmanX->AddComponent(new Collider2D());
	rockmanX->AddComponent(new RockManZ_Script());

	//collider2D = rockmanX->GetComponent<Collider2D>();
	//collider2D->SetOffsetPos(Vector3(0.0f, -10.0f, 0.0f));
	//collider2D->SetOffsetScale(Vector3(0.4f, 0.5f, 1.0f));
	//rockmanZ->GetComponent<Transform>()->SetScale(Vector3(2.0f, 2.0f, 0.0f));
	rockmanX->GetComponent<Transform>()->SetTranslation(Vector3(200.0f, 0.0f, 0.0f));

	rockmanX->GetComponent<Script>()->Initialize();
	rockmanX->GetComponent<Animator>()->Play();

	AddGameObject(rockmanX, 2, false);

	CollisionManager::GetInstance()->CheckLayer(1, 2);
}

void Scene::Update()
{
	for (auto& layer : this->m_layer_map)
		layer.second->Update();
}

void Scene::LateUpdate()
{
	for (auto& layer : this->m_layer_map)
		layer.second->LateUpdate();
}

void Scene::FinalUpdate()
{
	for (auto& layer : this->m_layer_map)
		layer.second->FinalUpdate();
}

void Scene::Render()
{
	auto graphis_manager = GraphicsManager::GetInstance();

	graphis_manager->BeginScene();

	for (auto& layer : this->m_layer_map)
		layer.second->Render();

	graphis_manager->EndScene();
}

void Scene::AddGameObject(GameObject* p_game_object, UINT layer_index, bool is_move)
{
	auto pair_iter = this->m_layer_map.insert(std::make_pair(layer_index, std::make_shared<Layer>(layer_index)));

	auto layer = pair_iter.first->second;
	layer->AddGameObject(p_game_object, is_move);
}

const std::shared_ptr<Layer>& Scene::GetLayer(const UINT& layer_index)
{
	auto layer_iter = this->m_layer_map.find(layer_index);

	if (layer_iter != this->m_layer_map.end())
		return layer_iter->second;

	return nullptr;
}
