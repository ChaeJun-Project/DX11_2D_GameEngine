#pragma once

class Scene;

class SceneManager final : public Singleton<SceneManager>
{
	SINGLETON(SceneManager);
	SceneManager();
	~SceneManager();

public:
    void Initialize();
    void Progress();

public:
    const std::shared_ptr<Scene>& GetCurrentScene() const { SAFE_GET_POINTER(this->m_p_current_scene); }

private: 
    std::shared_ptr<Scene> m_p_current_scene;
};

