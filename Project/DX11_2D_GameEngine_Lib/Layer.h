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
	void LateUpdate();
	void FinalUpdate();
	void Render();

public:
    void AddGameObject(GameObject* p_game_object, bool is_move);
	void RegisterObject(GameObject* p_game_object) { this->m_p_game_object_vector.emplace_back(p_game_object);}

	std::vector<GameObject*>& GetGameParentObjects() { return m_p_parent_game_object_vector; }
	std::vector<GameObject*>& GetGameObjects() { return m_p_game_object_vector; }

private:
    //�ش� layer�� ���� �ֻ��� �θ� ������Ʈ��
	std::vector<GameObject*> m_p_parent_game_object_vector;

	//�θ� �ڽ� ������� �ش� layer�� ���� ��� ������Ʈ��
	std::vector<GameObject*> m_p_game_object_vector;

	int m_layer_index = -1;
};

