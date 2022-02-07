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
   Component_Plus,
   Component_Back,
   Component_Transform,
   Component_Camera,
   Component_SpriteRenderer,
   Component_Animator2D,
   Component_Animator,
   Component_Collider2D,
   Component_Light2D,
   Component_ParticleSystem,
   Component_TileMapRenderer,
   Component_RigidBody2D,
   Component_Audio_Listener,
   Component_Audio_Source,

   Component_Script,

   //Console
   Console_Info,
   Console_Warning,
   Console_Error,
};

enum class FileThumbnailType : UINT
{
	None = 0,
	Folder = 1,
	Animation = 2,
	Audio = 3,
	Material = 4,
	Mesh = 5,
	Prefab = 6,
	Scene = 7,
	TileMap = 8,
	Texture = 9, //텍스처인 경우 자기 자신의 SRV로 설정
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
    void LoadToolBarIcons();
	void LoadGameObjectIcons();
	void LoadComponentIcons();
	void LoadConsoleIcons();
	void LoadFileThumbnails();

private:
	void LoadIconTexture(const std::string& texture_path, const IconType& icon_type);
	void LoadFileThumbnailTexture(const std::string& texture_path, const FileThumbnailType& file_thumbnail_type);

public:
	const std::shared_ptr<Texture>& GetIconTexture(const IconType& icon_type);
	const std::shared_ptr<Texture>& GetFileThumbnail(const FileThumbnailType& file_thumbnail_type);
	
public:
    void CreateImage(const IconType& icon_type, const ImVec2& button_size);
    const bool CreateImageButton(const IconType& icon_type, const ImVec2& button_size);

private:
    std::map<IconType, std::shared_ptr<Texture>> m_icon_map;
	std::map<FileThumbnailType, std::shared_ptr<Texture>> m_file_thumbnail_map;
};

