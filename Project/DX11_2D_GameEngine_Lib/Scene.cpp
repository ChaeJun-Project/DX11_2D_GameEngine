#include "stdafx.h"
#include "Scene.h"

#include "Layer.h"
#include "GameObject.h"

Scene::~Scene()
{
	for (auto& layer : this->m_layer_map)
		layer.second.reset();

	this->m_layer_map.clear();
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
