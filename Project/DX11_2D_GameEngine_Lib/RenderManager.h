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

private:
    void RenderTitle();
	void RenderPlay();
    void RenderEditor();

	void SetRenderTexture(const RenderTextureType& render_texture_type);

public:
	void CopyPostEffect();
	void ResizePostEffectTexture();

public:
    //Camera
	void RegisterEditorCamera(Camera* p_editor_camera) { m_p_editor_camera = p_editor_camera; }
	Camera* GetEditorCamera() { SAFE_GET_POINTER(m_p_editor_camera); }

    void RegisterCamera(Camera* p_camera, int& camera_index);
	Camera* GetMainCamera();

public:
	//Light2D
	void RegisterLight2D(Light2D* p_light2D, int& light2D_index);

	//Scene에 사용되는 모든 Light2D 데이터 상수 버퍼에 업데이트
	void UpdateConstantBuffer();

public:
    void SetResolution(const RenderTextureType& render_texture_type, const UINT& width, const UINT& height);

	const std::shared_ptr<Texture>& GetRenderTexture(const RenderTextureType& render_texture_type);

	const Vector2& GetResolution() { return m_resolution_size; }

private:
	void CreateRenderTexture(const RenderTextureType& render_texture_type, const UINT& width, const UINT& height);
	
private:
    //RenderTargetTexture
	std::map<RenderTextureType, std::shared_ptr<Texture>> m_p_render_texture_map;
	Vector2 m_resolution_size = Vector2::Zero;

    //Editor Camera
	Camera* m_p_editor_camera = nullptr;
    //Scene에 있는 Camera
    std::vector<Camera*> m_camera_vector;
	//Scene에 있는 Light2D
	std::vector<Light2D*> m_light2D_vector;

	//Post Effect 전용 Texture
	std::shared_ptr<Texture> m_p_post_effect_target_texture;
};

