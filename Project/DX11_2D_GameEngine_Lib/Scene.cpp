#include "stdafx.h"
#include "Scene.h"
#include "MainCamera.h"
#include "RockManZ.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	for(auto& object: m_object_list)
		object.reset();

	m_object_list.clear();
}

void Scene::Initialize()
{
	m_object_list.push_back(std::make_shared<MainCamera>());
	m_object_list.push_back(std::make_shared<RockManZ>());
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
	for (auto& object : m_object_list)
		object->Render();
}
