#pragma once

class GameObject;

class Layer final : public DX11Obejct
{
public:
	Layer(const UINT& layer_index);
	~Layer();

public:
	void Initialize();
	void Update();
	void FinalUpdate();
	void OnDisable();
	
public:
    void AddGameObject(GameObject* p_game_object, bool is_move);

	void RegisterGameObject(GameObject* p_game_object);
	void DeregisterGameObject(GameObject* p_game_object);
	void DeregisterFromParentGameObject(GameObject* p_game_object);

	const std::list<GameObject*>& GetParentGameObjects() { return m_p_parent_game_object_vector; }
	const std::list<GameObject*>& GetGameObjects() { return m_p_game_object_vector; }

	const bool GetLayerEmpty();

public:
	void SaveToScene(FILE* p_file) override {}
	void LoadFromScene(FILE* p_file) override {}

private: 
    //해당 Layer 번호
	UINT m_layer_index = 0;

    //해당 Layer에 속한 최상위 부모 GameObject들
	std::list<GameObject*> m_p_parent_game_object_vector;

	//부모 자식 관계없이 해당 Layer에 속한 모든 GameObject들
	std::list<GameObject*> m_p_game_object_vector;

	friend class Scene;
};

