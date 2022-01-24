#pragma once

class Scene;
class GameObject;

class SceneManager final : public Singleton<SceneManager>
{
	SINGLETON(SceneManager);
	SceneManager() = default;
	~SceneManager();

public:
	void Initialize();
	void Update();

	//юс╫ц
	void CreatePrefab(GameObject* p_game_object);

public:
    void SetCurrentScene(const std::shared_ptr<Scene>& p_current_scene);
	const std::shared_ptr<Scene>& GetCurrentScene() const { SAFE_GET_POINTER(m_p_current_scene); }

	const UINT& GetClientState() const { return m_client_state; }
	void SetClientState(const UINT& client_state) { m_client_state = client_state; }

	const UINT& GetEditorState() const { return m_editor_state; }
	void SetEditorState(const UINT& editor_state);

private:
	std::shared_ptr<Scene> m_p_current_scene;

	//<summary>
	//ClientState
	//Game = 1
	//Editor = 2
	//</summary>
	UINT m_client_state = 0;
	UINT m_editor_state = EditorState::EditorState_Stop;
};

