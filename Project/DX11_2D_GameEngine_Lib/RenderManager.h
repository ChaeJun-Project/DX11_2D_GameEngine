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

	//Scene�� ���Ǵ� ��� Light2D ������ ��� ���ۿ� ������Ʈ
	void UpdateConstantBuffer();

private:
    //Scene�� �ִ� Camera
    std::vector<Camera*> m_camera_vector;
	//Scene�� �ִ� Light2D
	std::vector<Light2D*> m_light2D_vector;

	//Post Effect ���� Texture
	std::shared_ptr<Texture> m_p_post_effect_target_texture;
};

