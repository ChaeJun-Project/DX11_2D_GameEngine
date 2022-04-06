#pragma once

class Scene;
class GameObject;
class Script;

typedef std::function<Script*(const std::string&)> GetScriptFunc;

class SceneManager final : public Singleton<SceneManager>
{
	SINGLETON(SceneManager);
	SceneManager() = default;
	~SceneManager();

public:
	void Initialize();

	void Awake();
	void Start();
	void Update();

public:
    void SetCurrentScene(const std::shared_ptr<Scene>& p_current_scene);
	std::shared_ptr<Scene>& GetCurrentScene() { return m_p_current_scene; }

	const UINT& GetClientState() const { return m_client_state; }
	void SetClientState(const UINT& client_state) { m_client_state = client_state; }

	const UINT& GetEditorState() const { return m_editor_state; }
	void SetEditorState(const UINT& editor_state);

public:
    void InitializeCurrentScene();
	void CreateNewScene();

	//Save Scene
public:
	bool SaveScene(const std::string& file_path);

private:
	void SaveGameObject(GameObject* p_game_object, FILE* p_file);
	void SaveScript(GameObject* p_game_object, FILE* p_file);

	//Load Scene
public:
	std::shared_ptr<Scene> LoadScene(const std::string& file_path);
	void SetGetScriptFunc(GetScriptFunc p_get_script_func) { m_p_get_script_func = p_get_script_func; }

private:
	GameObject* LoadGameObject(FILE* p_file);
	void LoadScript(GameObject* p_game_object, FILE* p_file);

public:
    void UpdateScene();
    
private:
	GetScriptFunc m_p_get_script_func = nullptr;

	std::shared_ptr<Scene> m_p_current_scene;

	//<summary>
	//ClientState
	//Game = 1
	//Editor = 2
	//</summary>
	UINT m_client_state = 0;
	UINT m_editor_state = EditorState::EditorState_Stop;
};

