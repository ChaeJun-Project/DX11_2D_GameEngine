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
	void RenderPlay();
	void RenderEditor();

	void CalcClientSceneRect();

	void ClearRenderTexture();
	void SetRenderTexture();

public:
	const bool CheckMouseWorldPositionInRect(const Vector2& mouse_position, const Vector2& rect_left_top, const Vector2& rect_right_bottom);
	const bool CheckMouseClientPositionInRect(const Vector2& mouse_position, const Vector2& rect_left_top, const Vector2& rect_right_bottom);
    const bool CheckClickedEditorSceneRect(const Vector2& mouse_position);
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
	const std::shared_ptr<Texture>& GetRenderTexture() { SAFE_GET_POINTER(m_p_render_texture); };

	void SetResolution(const UINT& width, const UINT& height);
	const Vector2& GetResolution() { return m_resolution_size; }
	
	void SetScreenOffset(const float& x, const float& y) { m_screen_offset.x = x; m_screen_offset.y = y;}
	const Vector2& GetScreenOffset() { return m_screen_offset; }

private:
	void CreateRenderTexture(const UINT& width, const UINT& height);

private:
	//RenderTargetTexture
	std::shared_ptr<Texture> m_p_render_texture = nullptr;
	Vector2 m_resolution_size = Vector2::Zero;
	Vector2 m_screen_offset = Vector2::Zero;
	Vector2 m_client_rect_left_top = Vector2::Zero;
	Vector2 m_client_rect_right_bottom = Vector2::Zero;

	//Editor Camera
	Camera* m_p_editor_camera = nullptr;
	//Scene에 있는 Camera
	std::vector<Camera*> m_camera_vector;
	//Scene에 있는 Light2D
	std::vector<Light2D*> m_light2D_vector;

	//Post Effect 전용 Texture
	std::shared_ptr<Texture> m_p_post_effect_target_texture;
};
