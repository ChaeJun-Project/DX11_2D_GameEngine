#pragma once

#include "stdafx.h"

class IComponent;

class GameObject : public DX11Obejct
{
public:
	GameObject() = default;
	explicit GameObject(const GameObject& origin); //복사생성자, 형변환 허용X
	~GameObject();

	void Start();
	void Update();
	void FinalUpdate();

	void Render();

private:
	template<typename T>
	static constexpr ComponentType GetComponentType();

public:
	void AddComponent(IComponent* p_component);

	template<typename T>
	T* GetComponent();
	IComponent* GetComponent(const ComponentType& component_type) const;

	void RemoveComponent(const ComponentType& component_type);

public:
	//Object IsActive
	bool& IsActive() { return m_active_check; }
	void SetObjectActive(const bool& is_active) { m_active_check = is_active; }

	//Dead Check
	const bool IsDead() { return m_dead_check; }

	//GameObject name
	const std::string& GetGameObjectName() const { return m_object_name; }
	void SetGameObjectName(const std::string& object_name) { m_object_name = object_name; }

	//GameObject Tag
	const std::string& GetGameObjectTag() const { return m_game_object_tag; }
	void SetGameObjectTag(const std::string& object_tag) { m_game_object_tag = object_tag; }
	
	//GameObject Layer
	const int& GetGameObjectLayer() const { return m_game_object_layer; }
	void SetGameObjectLayer(const UINT& layer_index) { m_game_object_layer = static_cast<UINT>(layer_index); }

public:
	//=====================================================================
	// [Hierarchy]
	//=====================================================================
	//재귀적인 방법으로 최상위 오브젝트 찾기
	GameObject* GetRoot();
	const bool& GetIsRoot() { return !HasParent(); }

	GameObject* GetParent() const { if(m_p_parent != nullptr) return m_p_parent; return nullptr;}
	
	const std::vector<GameObject*>& GetChilds() const { return m_p_child_vector; }
	GameObject* GetChildFromIndex(const UINT& index) const;
	GameObject* GetChildFromObjectName(const std::string& object_name) const;
	const bool GetHasChild(GameObject* p_game_object);
	const UINT& GetChildCount() const { return static_cast<UINT>(m_p_child_vector.size()); }

	void AddChild(GameObject* p_child_game_object);
	void DetachFromParent();
	
	const bool HasParent() { if(m_p_parent) return true; return false; }
	const bool HasChilds() { return !(m_p_child_vector.empty()); }

private:
    void SetDead() { m_dead_check = true; }
	void RegisterLayer();

public:
	GameObject* Clone() { return new GameObject(*this); }

	void RegisterPrefab();

protected:
    //Object Active Check
	bool m_active_check = true;
	//Object Dead Check
	bool m_dead_check = false;
	//GameObject Tag
	std::string m_game_object_tag;
	//GameObject Layer
	int m_game_object_layer = -1;

	//Component List
	std::list<std::pair<ComponentType, IComponent*>> m_component_list;
	
	//Script List
	std::list<Script*> m_script_list;

	//Hierarchy
	//Parent Object
	GameObject* m_p_parent = nullptr;
	//Child Object
	std::vector<GameObject*> m_p_child_vector;

	//해당 오브젝트로 프리팹을 만든 횟수
	UINT m_prefab_count = 0;

	friend class Layer;
	friend class EventManager;
};

template<typename T>
T* GameObject::GetComponent()
{
	//Class T가 IComponent를 상속받는 클래스인지 확인
	auto result = std::is_base_of<IComponent, T>::value;
	assert(result);
	if (!result)
	{
		return nullptr;
	}

	//타입 T에 해당하는 Shader Type 반환
	auto component_type = GetComponentType<T>();
	auto component = GetComponent(component_type);

	if (component != nullptr)
	{
		return dynamic_cast<T*>(component);
	}

	return nullptr;
}
