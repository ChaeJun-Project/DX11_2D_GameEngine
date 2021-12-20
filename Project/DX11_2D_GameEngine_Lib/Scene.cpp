#include "stdafx.h"
#include "Scene.h"

#include "Layer.h"
#include "GameObject.h"

#include "Layer.h"

Scene::Scene(const std::string& scene_name)
{
	m_object_name = scene_name;
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
		const std::vector<GameObject*> parent_game_objects = layer.second->GetParentGameObjects();

		for (UINT i = 0; i < static_cast<UINT>(parent_game_objects.size()); ++i)
		{
			m_p_parent_game_object_vector.emplace_back(parent_game_objects[i]);
		}
	}

	return m_p_parent_game_object_vector;
}

std::vector<GameObject*>& Scene::GetAllGameObjects()
{
	if (!m_p_game_object_vector.empty())
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
			if (game_objects[i]->GetGameObjectName() == game_object_name)
				return game_objects[i];
		}
	}

	return nullptr;
}

void Scene::CreateLayer(const UINT& layer_index)
{
	auto pair_iter = m_layer_map.insert(std::make_pair(layer_index, std::make_shared<Layer>(layer_index)));
	assert(pair_iter.second);
}

void Scene::AddGameObject(GameObject* p_game_object, UINT layer_index, bool is_move)
{
	std::shared_ptr<Layer> layer = nullptr;

	if (m_layer_map.find(layer_index) == m_layer_map.end())
		layer = m_layer_map.insert(std::make_pair(layer_index, std::make_shared<Layer>(layer_index))).first->second;

	layer = m_layer_map.find(layer_index)->second;
	layer->AddGameObject(p_game_object, is_move);
}

const std::shared_ptr<Layer>& Scene::GetLayer(const UINT& layer_index)
{
	auto layer_iter = m_layer_map.find(layer_index);

	if (layer_iter != m_layer_map.end())
		return layer_iter->second;

	return nullptr;
}

void Scene::SaveToScene(FILE* p_file)
{
	//Scene Name
	fprintf(p_file, "[Scene Name]\n");
	__super::SaveToScene(p_file);

	//Layer Count
	fprintf(p_file, "[Layer Count]\n");
	int layer_count = static_cast<int>(m_layer_map.size());
	fprintf(p_file, "%d\n", layer_count);

	//Layer Index List
	fprintf(p_file, "[Layer Index List]\n");
	for (const auto& layer : m_layer_map)
	{
		if (layer.second->m_layer_index != -1)
			fprintf(p_file, "%d ", layer.second->m_layer_index);
	}

	fprintf(p_file, "\n");
}

void Scene::LoadFromScene(FILE* p_file)
{
	char char_buffer[256] = {};
	FileManager::FScanf(char_buffer, p_file);
	FileManager::FScanf(char_buffer, p_file);
	m_object_name = char_buffer;

	//Layer Count
	int layer_count = 0;
	FileManager::FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%d\n", &layer_count);

	//Layer Index List
	FileManager::FScanf(char_buffer, p_file);
	for (int i = 0; i < layer_count; ++i)
	{
		int index = -1;
		fscanf_s(p_file, "%d", &index);
		if (index != -1)
			CreateLayer(static_cast<UINT>(index));
	}
}
