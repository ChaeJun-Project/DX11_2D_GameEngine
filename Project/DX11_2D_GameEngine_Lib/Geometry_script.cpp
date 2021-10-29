#include "stdafx.h"
#include "Geometry_script.h"

#include "Scene.h"

Geometry_script::Geometry_script()
{
}

Geometry_script::~Geometry_script()
{
}

void Geometry_script::Initialize()
{
	auto scene_manager = SceneManager::GetInstance();
	auto current_scene = scene_manager->GetCurrentScene();

	//BackGround(2)
	auto background = new GameObject();
	background->SetObjectName("Forest_Background");
	background->SetObjectTag("Background");
	background->AddComponent(new Transform());
	background->AddComponent(new Renderer());

	auto resource_manager = ResourceManager::GetInstance();

	auto renderer = background->GetComponent<Renderer>();
	auto material = renderer->GetMaterial();
	material->SetShader(resource_manager->GetShaderResource(ShaderResourceType::Standard_Light2D));
	material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, resource_manager->LoadTexture("Texture/Geometry/Forest.gif"));
	renderer->SetMesh(resource_manager->CreateMesh(resource_manager->GetTexture("Forest")->GetTextureSize()));

	background->GetComponent<Transform>()->SetScale(Vector3(6.0f, 6.0f, 1.0f));

	m_p_owner_game_object->AddChild(background);

	//Ground(2)
	auto ground = new GameObject();
	ground->SetObjectName("Forest_Ground");
	ground->SetObjectTag("Ground");
	ground->AddComponent(new Transform());
	ground->AddComponent(new Renderer());
	ground->AddComponent(new Collider2D());

	renderer = ground->GetComponent<Renderer>();
	material = renderer->GetMaterial();
	material->SetShader(resource_manager->GetShaderResource(ShaderResourceType::Standard_Light2D));
	material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, ResourceManager::GetInstance()->LoadTexture("Texture/Geometry/Ground.gif"));
	renderer->SetMesh(resource_manager->CreateMesh(resource_manager->GetTexture("Ground")->GetTextureSize()));
	
	auto collider2D = ground->GetComponent<Collider2D>();
	collider2D->SetOffsetPos(Vector3(0.0f, -0.16f, 0.0f));
	collider2D->SetOffsetScale(Vector3(2.6f, 0.31f, 1.0f));
	ground->GetComponent<Transform>()->SetScale(Vector3(2.0f, 2.0f, 1.0f));
	scene_manager->CreatePrefab(ground);
	
	auto ground_prefab = ResourceManager::GetInstance()->GetPrefab("Forest_Ground");

	for (int i = 0; i < 3; ++i)
	{
		auto ground = Instantiate(ground_prefab, Vector3((-1 + i)*(520.0f), -338.0f, 0.0f) , 2, false);
		m_p_owner_game_object->AddChild(ground);
	}
}

void Geometry_script::Update()
{
}

void Geometry_script::OnCollisionEnter(GameObject* other_game_object)
{
}

void Geometry_script::OnCollisionExit(GameObject* other_game_object)
{
}

void Geometry_script::OnCollision(GameObject* other_game_object)
{
}
