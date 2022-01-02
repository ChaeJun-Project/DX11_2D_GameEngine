#include "stdafx.h"
#include "Scene.h"

#include "Layer.h"
#include "GameObject.h"

#include "Transform.h"
#include "Camera.h"
#include "Light2D.h"

Scene::Scene(const std::string& scene_name)
{
	m_object_name = scene_name;

	//Create Layer(0 ~ 31)
	for (UINT i = 0; i < MAX_LAYER; ++i)
		CreateLayer(i);

	//Camera(0)
	auto camera = new GameObject();
	camera->SetGameObjectName("Main Camera");
	camera->SetGameObjectTag("Main Camera");
	camera->AddComponent(new Transform());
	camera->AddComponent(new Camera());

	camera->GetComponent<Camera>()->SetMainCamera();

	AddGameObject(camera);

	//Light2D
	auto point_light2D = new GameObject();
	point_light2D->SetGameObjectName("Light2D_Point");
	point_light2D->SetGameObjectTag("Light");
	point_light2D->AddComponent(new Transform());
	point_light2D->AddComponent(new Light2D());

	auto point_light = point_light2D->GetComponent<Light2D>();
	point_light->SetLightType(LightType::Point);
	point_light->SetLightRange(2000.0f);
	point_light->SetLightColor(Vector4::White);

	AddGameObject(point_light2D);
}

Scene::~Scene()
{
    //Layer
	for (auto& layer : m_layer_map)
		layer.second.reset();

	m_layer_map.clear();

	for (auto& p_parent_game_object : m_p_parent_game_object_vector)
		SAFE_DELETE(p_parent_game_object);

	m_p_parent_game_object_vector.clear();
	m_p_parent_game_object_vector.shrink_to_fit();
}

void Scene::Start()
{
	for (auto& layer : m_layer_map)
		layer.second->Start();
}

void Scene::Update()
{
	for (auto& layer : m_layer_map)
		layer.second->Update();
}

void Scene::FinalUpdate()
{
	for (auto& layer : m_layer_map)
		layer.second->FinalUpdate();
}

void Scene::AddGameObject(GameObject* p_new_game_object)
{
	if (p_new_game_object == nullptr)
		return;

	//해당 GameObject가 최상위 오브젝트일 경우
	if (!p_new_game_object->HasParent())
		m_p_parent_game_object_vector.emplace_back(p_new_game_object);

	m_p_game_object_vector.emplace_back(p_new_game_object);
	auto layer = p_new_game_object->GetGameObjectLayer();
	m_layer_map[layer]->RegisterGameObject(p_new_game_object);
}

GameObject* Scene::FindGameObject(const std::string& game_object_name)
{
	for (const auto& p_game_object : m_p_game_object_vector)
	{
		if (p_game_object->GetGameObjectName() == game_object_name)
			return p_game_object;
	}

	return nullptr;
}


const std::shared_ptr<Layer>& Scene::GetLayer(const UINT& layer_index)
{
	auto layer_iter = m_layer_map.find(layer_index);

	if (layer_iter != m_layer_map.end())
		return layer_iter->second;

	return nullptr;
}

const std::vector<GameObject*>& Scene::GetAllParentGameObjects()
{
	/*if (!m_p_parent_game_object_vector.empty())
		m_p_parent_game_object_vector.clear();

	for (auto& layer : m_layer_map)
	{
		const std::vector<GameObject*> parent_game_objects = layer.second->GetParentGameObjects();

		for (UINT i = 0; i < static_cast<UINT>(parent_game_objects.size()); ++i)
		{
			m_p_parent_game_object_vector.emplace_back(parent_game_objects[i]);
		}
	}*/

	return m_p_parent_game_object_vector;
}

const std::vector<GameObject*>& Scene::GetAllGameObjects()
{
	//if (!m_p_game_object_vector.empty())
	//	m_p_game_object_vector.clear();

	//for (auto& layer : m_layer_map)
	//{
	//	const std::vector<GameObject*> game_objects = layer.second->GetGameObjects();

	//	for (UINT i = 0; i < static_cast<UINT>(game_objects.size()); ++i)
	//	{
	//		m_p_game_object_vector.emplace_back(game_objects[i]);
	//	}
	//}

	return m_p_game_object_vector;
}

void Scene::CreateLayer(const UINT& layer_index)
{
	auto pair_iter = m_layer_map.insert(std::make_pair(layer_index, std::make_shared<Layer>(layer_index)));
	assert(pair_iter.second);
}

void Scene::SaveToScene(FILE* p_file)
{
	//Scene Name
	fprintf(p_file, "※ Scene\n");
	__super::SaveToScene(p_file);
}

void Scene::LoadFromScene(FILE* p_file)
{
	char char_buffer[256] = {};

	//Scene Name
	FileManager::FScanf(char_buffer, p_file);
	__super::LoadFromScene(p_file);
}
