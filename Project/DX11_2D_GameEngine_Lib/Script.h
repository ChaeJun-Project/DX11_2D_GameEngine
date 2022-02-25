#pragma once
#include "IComponent.h"

typedef std::variant<void*> ScriptParamData;
struct ScriptParamStruct
{
	ScriptParamStruct(const std::string& param_name, const ScriptParamType& param_type, ScriptParamData p_param_data)
		: m_param_name(param_name),
		m_param_type(param_type),
		m_p_param_data(p_param_data)
	{
	}

	std::string		m_param_name;
	ScriptParamType m_param_type;
	ScriptParamData	m_p_param_data;
};

class Prefab;
class GameObject;

class Script : public IComponent
{
public:
	Script(const std::string& script_name);
	virtual ~Script();

	virtual void Initialize() override {}
	virtual void Start() override {}
	virtual void Update() override {}
	virtual void FinalUpdate() final {} //Script를 상속받는 자식 클래스는 사용하지 않음

protected:
	//Script Param
	void AddScriptParamData(const ScriptParamStruct& script_param_data) { m_script_param_vector.emplace_back(script_param_data); }

	//GameObject
	GameObject* Instantiate(Prefab* p_game_object_prefab, const Vector3& position, const bool& use_event = false);
	void Destroy(GameObject* p_delete_game_object);
	void CreateGameObject(GameObject* p_new_game_object);

public:
	//Collision
	virtual void OnCollisionEnter(GameObject* other_game_object) {}
	virtual void OnCollisionStay(GameObject* other_game_object) {}
	virtual void OnCollisionExit(GameObject* other_game_object) {}

public:
	const std::string& GetScriptName() { return m_script_name; }
	const std::vector<ScriptParamStruct>& GetScriptParamVector() { return m_script_param_vector; }

public:
	virtual void SaveToScene(FILE* p_file) override;
	virtual void LoadFromScene(FILE* p_file) override = 0;

public:
	virtual Script* Clone() = 0;

protected:
	const std::string m_script_name;
	std::vector<ScriptParamStruct> m_script_param_vector;
};

