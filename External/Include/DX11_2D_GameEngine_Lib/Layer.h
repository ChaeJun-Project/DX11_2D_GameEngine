#pragma once

class GameObject;

class Layer final : public DX11Obejct
{
public:
	Layer(const UINT& layer_index);
	~Layer();

public:
	void Start();
	void Update();
	void FinalUpdate();
	void Render();

public:
    void AddGameObject(GameObject* p_game_object, bool is_move);

	void RegisterGameObject(GameObject* p_game_object);
	void DeregisterGameObject(GameObject* p_game_object);
	void DeregisterFromParentGameObject(GameObject* p_game_object);

	const std::vector<GameObject*>& GetParentGameObjects() { return m_p_parent_game_object_vector; }
	const std::vector<GameObject*>& GetGameObjects() { return m_p_game_object_vector; }

public:
	void SaveToScene(FILE* p_file) override {}
	void LoadFromScene(FILE* p_file) override {}

private: 
    //�ش� Layer ��ȣ
	UINT m_layer_index = 0;

    //�ش� Layer�� ���� �ֻ��� �θ� GameObject��
	std::vector<GameObject*> m_p_parent_game_object_vector;

	//�θ� �ڽ� ������� �ش� Layer�� ���� ��� GameObject��
	std::vector<GameObject*> m_p_game_object_vector;

	friend class Scene;
};

