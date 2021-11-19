#pragma once
#include "GUI/IGUI.h"

class Transform;
class Camera;
class SpriteRenderer;
class Animater;
class Script;
class Collider2D;
class Light2D;
class ParticleSystem;

class GameObject;

class IResource;

class GUI_Inspector : public IGUI
{
public:
	GUI_Inspector(const std::string& inspector_title);
	~GUI_Inspector() = default;

public:
	void Update() override;

	void Render() override;

	//=====================================
	//Component
	//=====================================
private:
	const bool BeginComponent(const std::string component_name, const IconType& icon_type);
	void ShowComponentSettingPopup();
	void DrawComponentEnd();

private:
	void ShowTransformComponent(const Transform* transform);
	void ShowCameraComponent(const Transform* transform);
	void ShowSpriteRenderer(const SpriteRenderer* sprite_renderer);
	void ShowAnimaterComponent(const Animater* animator);
	void ShowScriptComponent(const Script* script);
	void ShowCollider2DComponent(const Collider2D* transform);
	void ShowLight2DComponent(const Light2D* transform);
	void ShowParticleSystemComponent(const ParticleSystem* transform);

	void ShowAddComponent();
	void ShowAddComponentPopup();

	//=====================================
	//Resource Info
	//=====================================
private:


private:
	std::weak_ptr<GameObject> m_select_game_object;

	std::weak_ptr<IResource> m_select_resource;
};

