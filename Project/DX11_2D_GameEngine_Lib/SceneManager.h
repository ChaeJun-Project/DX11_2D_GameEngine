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
    const std::shared_ptr<Scene>& GetCurrentScene() const { SAFE_GET_POINTER(this->m_p_current_scene); }

    void SetClientState(const UINT& client_state) { m_client_state = client_state; }
    
    const EditorState& GetEditorState() const { return m_editor_state; }
    void SetEditorState(const EditorState& editor_state) { m_editor_state = editor_state; }


private: 
    std::shared_ptr<Scene> m_p_current_scene;


    UINT m_client_state = 0;
    EditorState m_editor_state = EditorState::EditorState_Stop;
};

