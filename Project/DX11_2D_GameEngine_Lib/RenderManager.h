#pragma once

class Camera;
class Light2D;

class Texture;

class RenderManager final : public Singleton<RenderManager>
{
	SINGLETON(RenderManager);

	RenderManager();
	~RenderManager();

public:
    void Initialize();
    void Render();

public:
	void CopyPostEffect();
	void ResizePostEffectTexture();

public:
    //Camera
    void RegisterCamera(Camera* p_camera, int& camera_index);

	Camera* GetMainCamera();

public:
	//Light2D
	void RegisterLight2D(Light2D* p_light2D, int& light2D_index);

	//Scene에 사용되는 모든 Light2D 데이터 상수 버퍼에 업데이트
	void UpdateConstantBuffer();

private:
    //Scene에 있는 Camera
    std::vector<Camera*> m_camera_vector;
	//Scene에 있는 Light2D
	std::vector<Light2D*> m_light2D_vector;

	//Post Effect 전용 Texture
	std::shared_ptr<Texture> m_p_post_effect_target_texture;
};

