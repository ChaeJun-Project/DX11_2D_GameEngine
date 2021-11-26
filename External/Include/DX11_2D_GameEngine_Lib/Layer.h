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
    //해당 layer에 속한 최상위 부모 오브젝트들
	std::vector<GameObject*> m_p_parent_game_object_vector;

	//부모 자식 관계없이 해당 layer에 속한 모든 오브젝트들
	std::vector<GameObject*> m_p_game_object_vector;

	int m_layer_index = -1;
};

