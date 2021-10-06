#pragma once

class Scene;

class SceneManager final : public Singleton<GraphicsManager>
{
	SINGLETON(SceneManager);
	SceneManager();
	~SceneManager();

public:
    void Initialize();
    void Progress();

private: 
    std::shared_ptr<Scene> m_p_current_scene;
};

