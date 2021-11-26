#include "stdafx.h"
#include "Scene.h"

#include "Layer.h"
#include "GameObject.h"

#include "Layer.h"

Scene::Scene(const std::string& scene_name)
	: m_scene_name(scene_name)
{
}

Scene::~Scene()
{
	for (auto& layer : m_layer_map)
		layer.second.reset();

	m_layer_map.clear();
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

void Scene::LateUpdate()
{
	for (auto& layer : m_layer_map)
		layer.second->LateUpdate();
}

void Scene::FinalUpdate()
{
	for (auto& layer : m_layer_map)
		layer.second->GetGameObjects().clear();

	for (auto& layer : m_layer_map)
		layer.second->FinalUpdate();
}

std::vector<GameObject*>& Scene::GetAllParentGameObjects()
{
	if (!m_p_parent_game_object_vector.empty())
		m_p_parent_game_object_vector.clear();

	for (auto& layer : m_layer_map)
	{
		const std::vector<GameObject*> parent_game_objects = layer.second->GetGameParentObjects();

		for (UINT i = 0; i < static_cast<UINT>(parent_game_objects.size()); ++i)
		{
			m_p_parent_game_object_vector.emplace_back(parent_game_objects[i]);
		}
	}

	return m_p_parent_game_object_vector;
}

std::vector<GameObject*>& Scene::GetAllGameObjects()
{
	if(!m_p_game_object_vector.empty())
		m_p_game_object_vector.clear();

	for (auto& layer : m_layer_map)
	{
		const std::vector<GameObject*> game_objects = layer.second->GetGameObjects();

		for (UINT i = 0; i < static_cast<UINT>(game_objects.size()); ++i)
		{
			m_p_game_object_vector.emplace_back(game_objects[i]);
		}
	}

	return m_p_game_object_vector;
}

GameObject* Scene::FindGameObjectByName(const std::string& game_object_name)
{
	for (auto& layer : m_layer_map)
	{
		const std::vector<GameObject*> game_objects = layer.second->GetGameObjects();

		for (UINT i = 0; i < static_cast<UINT>(game_objects.size()); ++i)
		{
			if (game_objects[i]->GetObjectName() == game_object_name)
				return game_objects[i];
		}
	}

	return nullptr;
}

void Scene::AddGameObject(GameObject* p_game_object, UINT layer_index, bool is_move)
{
	auto pair_iter = m_layer_map.insert(std::make_pair(layer_index, std::make_shared<Layer>(layer_index)));

	auto layer = pair_iter.first->second;
	layer->AddGameObject(p_game_object, is_move);
}

const std::shared_ptr<Layer>& Scene::GetLayer(const UINT& layer_index)
{
	auto layer_iter = m_layer_map.find(layer_index);

	if (layer_iter != m_layer_map.end())
		return layer_iter->second;

	return nullptr;
}
