#pragma once

enum class IconType : UINT
{
   //Tool Bar
   ToolBar_Play,
   ToolBar_Pause,
   ToolBar_Stop,

   //GameObject
   Inspector_GameObject,
   Inspector_Prefab_GameObject,

   //Component
   Component_Settings,
   Component_Transform,
   Component_Camera,
   Component_SpriteRenderer,
   Component_Animator,
   Component_Script,
   Component_Collider2D,
   Component_Light2D,
   Component_ParticleSystem,
   Component_RigidBody2D,

   //Console
   Console_Info,
   Console_Warning,
   Console_Error,
};

#include <DX11_2D_GameEngine_Lib/Singleton.h>
#include <DX11_2D_GameEngine_Lib/Texture.h>

class IconProvider final : public Singleton<IconProvider>
{
	SINGLETON(IconProvider);

	IconProvider() = default;
	~IconProvider();

public:
    void Initialize();

private:
	void LoadIconTexture(const std::string& texture_path, const IconType& icon_type);
	const std::shared_ptr<Texture>& GetIconTexture(const IconType& icon_type);

public:
    void CreateImage(const IconType& icon_type, const ImVec2& button_size);
    const bool CreateImageButton(const char* id, const IconType& icon_type, const ImVec2& button_size);

private:
    std::map<IconType, std::shared_ptr<Texture>> m_icon_map;

};

