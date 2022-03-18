#pragma once

#include "stdafx.h"

class IComponent;
class Script;

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

	//==============
	// [Component]
	//==============
private:
	template<typename T>
	static constexpr ComponentType GetComponentType();

public:
	void AddComponent(const ComponentType& component_type);
	void AddComponent(IComponent* p_component);
	void AddRectTransform();
	void AddImageRendererComponent();

	template<typename T>
	T* GetComponent();
	IComponent* GetComponent(const ComponentType& component_type) const;

	void RemoveComponent(const ComponentType& component_type);
	void RemoveAllComponent();
	
	const UINT& GetComponentCount() const { return static_cast<UINT>(m_p_component_map.size()); }

	//==============
	// [Script]
	//==============
public:
	const std::unordered_map<std::string, Script*>& GetScriptUnMap() { return m_p_script_un_map; }

    Script* GetScript(const std::string& script_name);

	void RemoveScript(const std::string& script_name);

	const UINT& GetScriptCount() const { return static_cast<UINT>(m_p_script_un_map.size()); }

public:
	//GameObject IsActive
	const bool& GetIsActive() { return m_is_active; }
	void SetIsActive(const bool& is_active) { m_is_active = is_active; }

	//Dead Check
	const bool IsDead() { return m_dead_check; }

	//GameObject name
	const std::string& GetGameObjectName() const { return m_object_name; }
	void SetGameObjectName(const std::string& object_name) { m_object_name = object_name; }

	//GameObject Tag
	const std::string& GetGameObjectTag() const { return m_game_object_tag; }
	void SetGameObjectTag(const std::string& object_tag) { m_game_object_tag = object_tag; }

	//GameObject Layer
	const UINT& GetGameObjectLayer() const { return m_game_object_layer; }
	void SetGameObjectLayer(const UINT& layer_index);

public:
	//=====================================================================
	// [Hierarchy]
	//=====================================================================
	//재귀적인 방법으로 최상위 오브젝트 찾기
	GameObject* GetRoot();
	const bool& GetIsRoot() { return !HasParent(); }

	GameObject* GetParent() const { if (m_p_parent != nullptr) return m_p_parent; return nullptr; }

	const std::vector<GameObject*>& GetChilds() const { return m_p_child_vector; }
	GameObject* GetChildFromIndex(const UINT& index) const;
	GameObject* GetChildFromObjectName(const std::string& object_name) const;
	const bool GetHasChild(GameObject* p_game_object);
	const UINT& GetChildCount() const { return static_cast<UINT>(m_p_child_vector.size()); }

	void AddChild(GameObject* p_child_game_object);
	void DetachFromParent();

	bool HasParent() { if (m_p_parent) return true; return false; }
	bool HasChilds() { return !(m_p_child_vector.empty()); }

private:
	void SetDead() { m_dead_check = true; }

public:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	GameObject* Clone() { return new GameObject(*this); }

protected:
	//GameObject Active Check
	bool m_is_active = true;
	//GameObject Dead Check
	bool m_dead_check = false;
	//GameObject Tag
	std::string m_game_object_tag = "Default";
	//GameObject Layer
	UINT m_game_object_layer = 0;

	//Component Map
	std::map<ComponentType, IComponent*> m_p_component_map;

	//Script Map
	std::unordered_map<std::string, Script*> m_p_script_un_map;

	//Hierarchy
	//Parent GameObject
	GameObject* m_p_parent = nullptr;
	//Child GameObject
	std::vector<GameObject*> m_p_child_vector;

	friend class Layer;
	friend class EventManager;
};

template<typename T>
T* GameObject::GetComponent()
{
	//Class T가 IComponent를 상속받는 클래스가 아니고 또는 Class T가 Script인 경우
	if ((!std::is_base_of<IComponent, T>::value) || std::is_same<Script, T>::value)
		return nullptr;

	//타입 T에 해당하는 Shader Type 반환
	auto component_type = GetComponentType<T>();
	auto component = GetComponent(component_type);

	if (component != nullptr)
	{
		return dynamic_cast<T*>(component);
	}

	return nullptr;
}
