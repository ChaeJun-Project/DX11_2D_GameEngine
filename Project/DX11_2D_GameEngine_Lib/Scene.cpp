#include "stdafx.h"
#include "Scene.h"

#include "Layer.h"
#include "GameObject.h"

#include "AudioClip.h"

#include "Camera.h"
#include "Light2D.h"
#include "AudioSource.h"

Scene::Scene(const std::string& scene_name)
{
	m_object_name = scene_name;

	//Create Layer(0 ~ 31)
	for (UINT i = 0; i < MAX_LAYER; ++i)
		CreateLayer(i);
}

Scene::~Scene()
{
    //Clear Layer Map
	for (auto& layer : m_layer_map)
		layer.second.reset();

	m_layer_map.clear();

	//Clear GameObject Vector
	m_p_game_object_vector.clear();
	m_p_game_object_vector.shrink_to_fit();

	//Clear Parent GameObject Vector
	for (auto& p_parent_game_object : m_p_parent_game_object_vector)
		SAFE_DELETE(p_parent_game_object);

	m_p_parent_game_object_vector.clear();
	m_p_parent_game_object_vector.shrink_to_fit();
}

void Scene::Initialize()
{
	//Camera(0)
	auto camera = new GameObject();
	camera->SetGameObjectName("Main Camera");
	camera->SetGameObjectTag("Main Camera");
	camera->AddComponent(ComponentType::Transform);
	camera->AddComponent(ComponentType::Camera);
	camera->AddComponent(ComponentType::AudioListener);

	camera->GetComponent<Camera>()->SetMainCamera();

	RegisterGameObject(camera);

	//Light2D
	auto point_light2D = new GameObject();
	point_light2D->SetGameObjectName("Light2D_Point");
	point_light2D->SetGameObjectTag("Light");
	point_light2D->AddComponent(ComponentType::Transform);
	point_light2D->AddComponent(ComponentType::Light2D);

	auto point_light = point_light2D->GetComponent<Light2D>();
	point_light->SetLightType(LightType::Point);
	
	RegisterGameObject(point_light2D);

	//Test
	/*auto test = new GameObject();
	test->SetGameObjectName("SpriteRenderer");
	test->AddComponent(ComponentType::Transform);
	test->AddComponent(ComponentType::SpriteRenderer);

	auto sprtie_renderer = test->GetComponent<SpriteRenderer>();
	sprtie_renderer->SetSpriteTexture(ResourceManager::GetInstance()->CreateTexture((FILE_MANAGER->GetAbsoluteContentPath() + "Asset/Texture/UI/PlayerSelect/Z_Texture.png")));
	
	RegisterGameObject(test);

	auto audio = new GameObject();
	audio->SetGameObjectName("Audio");
	audio->AddComponent(ComponentType::Transform);
	audio->AddComponent(ComponentType::AudioSource);

	auto audio_source = audio->GetComponent<AudioSource>();
	audio_source->SetAudioClip(ResourceManager::GetInstance()->LoadFromFile<AudioClip>((FILE_MANAGER->GetAbsoluteContentPath() + "Asset/Audio/17 - X4 - BOSS.mp3")));

	RegisterGameObject(audio);*/
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

void Scene::RegisterGameObject(GameObject* p_game_object)
{
	if (p_game_object == nullptr)
		return;

	//해당 GameObject가 최상위 오브젝트일 경우
	if (!p_game_object->HasParent())
		m_p_parent_game_object_vector.emplace_back(p_game_object);

	m_p_game_object_vector.emplace_back(p_game_object);
	auto layer = p_game_object->GetGameObjectLayer();
	m_layer_map[layer]->RegisterGameObject(p_game_object);

	//해당 GameObject가 자식을 가지고 있을 경우
	const auto& child_game_object_vector = p_game_object->GetChilds();
	for (const auto& p_child_game_object : child_game_object_vector)
		RegisterGameObject(p_child_game_object);
}

void Scene::DeregisterGameObject(GameObject* p_game_object)
{
	//해당 GameObject 부모가 없는 최상위 오브젝트라면
	if (!p_game_object->HasParent())
		DeregisterFromParentGameObject(p_game_object);

	std::vector<GameObject*>::iterator iter = m_p_game_object_vector.begin();

	for (; iter != m_p_game_object_vector.end();)
	{
		if ((*iter) == p_game_object)
		{
			iter = m_p_game_object_vector.erase(iter);
			break;
		}

		else
			++iter;
	}

	auto layer = p_game_object->GetGameObjectLayer();
	m_layer_map[layer]->DeregisterGameObject(p_game_object);
}

void Scene::DeregisterFromParentGameObject(GameObject* p_game_object)
{
	std::vector<GameObject*>::iterator iter = m_p_parent_game_object_vector.begin();

	for (; iter != m_p_parent_game_object_vector.end();)
	{
		if ((*iter) == p_game_object)
		{
			iter = m_p_parent_game_object_vector.erase(iter);
			return;
		}

		else
			++iter;
	}
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
	FILE_MANAGER->FScanf(char_buffer, p_file);
	__super::LoadFromScene(p_file);
}
