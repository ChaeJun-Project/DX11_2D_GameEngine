#pragma once
#include "IComponent.h"

typedef std::variant<void*, std::shared_ptr<Texture>*, std::shared_ptr<Prefab>*> ScriptParamData;
struct ScriptParamStruct
{
	ScriptParamStruct(const std::string& param_name, const ScriptParamType& param_type, ScriptParamData p_param_data, const float& indent)
		: m_param_name(param_name),
		m_param_type(param_type),
		m_p_param_data(p_param_data),
		m_indent(indent)
	{
	}

	std::string		m_param_name;
	ScriptParamType m_param_type;
	ScriptParamData	m_p_param_data;
	float			m_indent = 0.0f; //파라미터 이름과 입력칸 사이의 여백 너비
};

class Prefab;
class GameObject;

class Script : public IComponent
{
public:
	Script(const std::string& script_name);
	virtual ~Script();

	virtual void Awake() override {}
	virtual void OnEnable() override {}
	virtual void Start() override {}
	virtual void Update() override {}
	virtual void OnDisable() override {}

	virtual void FinalUpdate() final {} //Script를 상속받는 자식 클래스는 사용하지 않음

protected:
	virtual void RegisterScriptParamData() {}
	//Script Param
	void AddScriptParamData(const ScriptParamStruct& script_param_data) { m_script_param_vector.emplace_back(script_param_data); }

	//GameObject
	GameObject* Instantiate(const std::shared_ptr<Prefab>& p_game_object_prefab, const Vector3& position = Vector3::Zero);
	void OnDestroy(GameObject* p_delete_game_object);

private:
	void CreateGameObject(GameObject* p_new_game_object);

public:
	//Collision
	//처음으로 충돌이 시작된 경우 호출
	virtual void OnCollisionEnter(GameObject* other_game_object) {}
	//이전에 충돌했고 충돌이 유지된 경우 호출
	virtual void OnCollisionStay(GameObject* other_game_object) {}
	//이전에 충돌했으나 지금은 충돌이 끝난 경우
	virtual void OnCollisionExit(GameObject* other_game_object) {}

public:
	const std::string GetScriptName() const { return m_script_name; }
	const std::vector<ScriptParamStruct>& GetScriptParamVector() const { return m_script_param_vector; }

public:
	virtual void SaveToScene(FILE* p_file) override;
	virtual void LoadFromScene(FILE* p_file) override = 0;

public:
	virtual Script* Clone() const = 0;

protected:
	const std::string m_script_name;
	std::vector<ScriptParamStruct> m_script_param_vector;
};

