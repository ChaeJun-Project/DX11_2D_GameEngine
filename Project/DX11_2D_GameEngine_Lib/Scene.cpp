#include "stdafx.h"
#include "Scene.h"
#include "MainCamera.h"
#include "GameObject.h"

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
	//TODO
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
