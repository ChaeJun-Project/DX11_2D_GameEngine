#pragma once

class GameObject;

class Layer final : public DX11Obejct
{
public:
	Layer(const UINT& layer_index);
	~Layer();

public:
	void Update();
	void LateUpdate();
	void FinalUpdate();
	void Render();

public:
    void AddGameObject(const std::shared_ptr<GameObject>& p_game_object, bool is_move);
	void RegisterObject(const std::shared_ptr<GameObject>& p_game_object) { this->m_p_game_object_list.push_back(p_game_object);}

private:
    //해당 layer에 속한 최상위 부모 오브젝트들
	std::list<std::shared_ptr<GameObject>> m_p_parent_game_object_list;

	//부모 자식 관계없이 해당 layer에 속한 모든 오브젝트들
	std::list<std::weak_ptr<GameObject>> m_p_game_object_list;

	int m_layer_index = -1;
};

