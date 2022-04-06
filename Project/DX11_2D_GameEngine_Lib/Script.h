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
	float			m_indent = 0.0f; //�Ķ���� �̸��� �Է�ĭ ������ ���� �ʺ�
};

class Prefab;
class GameObject;

class Script : public IComponent
{
public:
	Script(const std::string& script_name);
	virtual ~Script();

	virtual void Awake() {}
	virtual void OnEnable() {}
	virtual void OnDisable() {}
	virtual void Start() override {}
	virtual void Update() override {}
	virtual void FinalUpdate() final {} //Script�� ��ӹ޴� �ڽ� Ŭ������ ������� ����

protected:
	virtual void RegisterScriptParamData() {}
	//Script Param
	void AddScriptParamData(const ScriptParamStruct& script_param_data) { m_script_param_vector.emplace_back(script_param_data); }

	//GameObject
	GameObject* Instantiate(const std::shared_ptr<Prefab>& p_game_object_prefab, const Vector3& position = Vector3::Zero);
	void OnDestroy(GameObject* p_delete_game_object);
	void CreateGameObject(GameObject* p_new_game_object);

public:
	//Collision
	virtual void OnCollisionEnter(GameObject* other_game_object) {}
	virtual void OnCollisionStay(GameObject* other_game_object) {}
	virtual void OnCollisionExit(GameObject* other_game_object) {}

public:
	void SetIsActive(const bool& is_active) override;

	void AddStartFuncCallCount() { if (m_start_func_call_count == 0) ++m_start_func_call_count; }
	const UINT GetStartFuncCallCount() const { return m_start_func_call_count; }

	const std::string GetScriptName() const { return m_script_name; }
	const std::vector<ScriptParamStruct>& GetScriptParamVector() const { return m_script_param_vector; }

public:
	virtual void SaveToScene(FILE* p_file) override;
	virtual void LoadFromScene(FILE* p_file) override = 0;

public:
	virtual Script* Clone() const = 0;

protected:
	UINT m_start_func_call_count = 0;

	const std::string m_script_name;
	std::vector<ScriptParamStruct> m_script_param_vector;
};

