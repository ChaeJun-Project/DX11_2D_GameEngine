#pragma once

class Camera;
class Light2D;

class RenderManager final : public Singleton<RenderManager>
{
	SINGLETON(RenderManager);

	RenderManager();
	~RenderManager();

public:
    void Render();

public:
    void RegisterCamera(Camera* p_camera, int camera_index);

private:
    //Scene�� �ִ� Camera
    std::vector<Camera*> m_camera_vector;
	//Scene�� �ִ� Light2D
	std::vector<Light2D*> m_light2D_vector;
};

